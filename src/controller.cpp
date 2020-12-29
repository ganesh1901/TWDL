/* Controller to access all application
Build Revision :
#Build Date :
#Change Log :
#Build By   :  aviral  */


#include <stdemace.h>
#include<DIP.h>
#include<DOP.h>
#include<ADC.h>

#include<RS422.h>
#include<unistd.h>
#include<controller.h>


DIP *DIP::dip_instance=0;
DOP *DOP::dop_instance=0;
ADC *ADC::adc_instance=0;
DDC1553 *DDC1553::ddc1553_instance=0;
RS422 *RS422::rs422_instance=0;

DOP *dop;
DIP *dip;
ADC *adc;
DDC1553 *ddc;
TimerThread *timerTh;
Controller controller;



void LogwithTimestamp(char *msg){
	time_t t = std::time(0);   // get time now
	tm* now = std::localtime(&t);
	fprintf(stdout, " ***** [%d %d:%d:%d ] %s ****\n", now->tm_yday, now->tm_hour, now->tm_min, now->tm_sec, msg);
}


void send1553FlatBufferObject(int index, int pos)
{	
	uint8_t buffer[64];
	flatbuffers::FlatBufferBuilder builder;

	memcpy(buffer, device_params[index]->payload, sizeof buffer);

#if 0
	fprintf(stdout, "1553 Data \n ");
	for(int i=0; i<32; i++){
		fprintf(stdout, " %02X", device_params[index]->payload[i]);
		if((i % 8) == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
#endif

	auto payload = builder.CreateVector(buffer , 64);
	auto ors = CreateMil_1553( builder , device_params[index]->Rtnum, device_params[index]->Subaddr, device_params[index]->direction,
				device_params[index]->wordcount, payload, device_params[index]->frequency);   // MIL1553 schema model created
	auto tran_obj = CreateTransaction(builder, IdentifyHardwareInterface_Mil_1553 , ors.Union());
	builder.Finish(tran_obj);  // Serialize the root of the object.

	int FILEID = 0;
	if(pos == 1)
		FILEID = controller.SUR_PIPE;
	else if(pos == 2)
		FILEID = controller.EMA_PIPE;
	else
		FILEID = controller.RSP_PIPE;

	write(FILEID, builder.GetBufferPointer(),  builder.GetSize());

}


void sendRRUFlatBufferObject(unsigned short rru_type)
{
	flatbuffers::FlatBufferBuilder builder;
	auto ors = CreateIP_RRU(builder, rru_type, controller.RRUPacket[rru_type].slot, controller.RRUPacket[rru_type].port, controller.RRUPacket[rru_type].channel, controller.RRUPacket[rru_type].value, controller.RRUPacket[rru_type].value2);
	auto tran_obj = CreateTransaction(builder, IdentifyHardwareInterface_IP_RRU ,ors.Union());  
  	builder.Finish(tran_obj);
	write(controller.RSP_PIPE, builder.GetBufferPointer(),  builder.GetSize());
}



void getFlatBufferObject(unsigned char buffer_read[]) {
	try{
	   unsigned char ddc_buffer[64] = {};
	   short payload_size = 0;
	   short direction = 0;
	   int i;

	   auto trans = GetTransaction( buffer_read );
	   if(trans->queryOnInterface_type() == IdentifyHardwareInterface_Mil_1553) {

			printf("Got FlatBuffer Object\n");

			auto interfaceType = static_cast<const Mil_1553 *> (trans->queryOnInterface());

			direction = interfaceType->direction();
			cout<<"******************************Received 1553 Data From Python GUI***********************************\n" ;
			device_params[direction]->Rtnum     = interfaceType->rt();
			device_params[direction]->Subaddr   = interfaceType->subaddress();
			device_params[direction]->direction = direction;
			device_params[direction]->wordcount = interfaceType->wordcount();
			device_params[direction]->frequency = interfaceType->frequency();
			payload_size = interfaceType->payload()->size();
	
			cout << " direction "<< direction << endl;
			//cout<<"interfaceType->direction()=="<<interfaceType->direction()<< " word count"<<  interfaceType->wordcount()<< endl;

			if (direction == 0)
			{

		#if 1
				for(i=0; i<payload_size; i++)
				{
					ddc_buffer[i] = interfaceType->payload()->Get(i);
					//printf(" %x", ddc_buffer[i]);

				}
				/* copying to device structure */
				memcpy(&device_params[direction]->payload, &ddc_buffer, payload_size);
		#else
				memset(ddc_buffer, 0, sizeof ddc_buffer);
				memcpy(&ddc_buffer, interfaceType->payload()+2, payload_size);
				memcpy(device_params[direction]->payload, ddc_buffer, sizeof ddc_buffer);
		#endif
			}

			if(direction >= 0 && direction < MAX_SUBADDRESS)
			{
				//cout << "In direction check " << endl;
				controller.FunctionPointer[direction]();
			}
			else{
				fprintf(stdout, " Invalid OpCode Received discarded !!\n");
			}

	   }
	   else if(trans->queryOnInterface_type() == IdentifyHardwareInterface_IP_RRU){
			cout<<"******************************Received RRU  data From Python GUI***********************************\n";
			auto interfaceType = static_cast<const IP_RRU *> (trans->queryOnInterface());


			controller.RRUPacket[interfaceType->i_type()].i_type=interfaceType->i_type();
			controller.RRUPacket[interfaceType->i_type()].slot=interfaceType->slot();
			controller.RRUPacket[interfaceType->i_type()].port=interfaceType->port();
			controller.RRUPacket[interfaceType->i_type()].channel=interfaceType->channel();
			controller.RRUPacket[interfaceType->i_type()].value=interfaceType->value();
			controller.RRUPacket[interfaceType->i_type()].value2=interfaceType->value2();


			if ( interfaceType->i_type() == 1){
				//(slot, port, channel, on/off)  on-1 & off-0
				dop->writeToDo_SLOT(interfaceType->slot(), interfaceType->port(), interfaceType->channel(), interfaceType->value());
			}
			else if ( interfaceType->i_type() == 0){
				controller.RRUPacket[interfaceType->i_type()].value = dip->read_DI(interfaceType->slot(), interfaceType->port(), interfaceType->channel());
				sendRRUFlatBufferObject(interfaceType->i_type());
			}
			else{
				controller.RRUPacket[interfaceType->i_type()].value2 = adc->readADCVolatage(interfaceType->channel());
				sendRRUFlatBufferObject(interfaceType->i_type());
			}
	   }
	}
	catch(exception ex){
			cout << "Exception type" << ex.what() << endl;
	}
}


unsigned long gettimestamp(void){
	gettimeofday(&time1, NULL);
	return ((time1.tv_sec * 1000000)+time1.tv_usec);
}


void TransmitCommand()
{
	//cout << " In Transmit command " << endl;


	ddc->async = TRUE;
	ddc->scheduleAsyncTransmitMsg(device_params[1]);
	ddc->async = FALSE;

	unsigned long t1 = gettimestamp();
	memset(buffer, 0, sizeof buffer);
	memcpy(&buffer, &t1, sizeof t1);
	memcpy(&buffer[8], &device_params[1]->CommandWord, sizeof device_params[1]->CommandWord);
	memcpy(&buffer[10], &device_params[1]->payload, sizeof device_params[1]->payload);
	write(controller.LOG_FP, buffer, sizeof buffer);
	send1553FlatBufferObject(1, 0);

}


void ReceiveCommand()
{
	unsigned long t1 = gettimestamp();

	//pthread_mutex_lock(&mutex);
	

	memset(buffer, 0, sizeof buffer);
	memcpy(&buffer, &t1, sizeof t1);
	memcpy(&buffer[8], &device_params[0]->CommandWord, sizeof device_params[0]->CommandWord);
	memcpy(&buffer[10], &device_params[0]->payload, sizeof device_params[0]->payload);

	write(controller.LOG_FP, buffer, sizeof buffer);
	ddc->scheduleAsyncReceiveMsg(device_params[0]);

}


void dummy(){

}


void INS_Survelliance_Start(void){
	controller.ins_survelliance = true;
}

void INS_Survelliance_Stop(void){
	controller.ins_survelliance = false;
}


void OBC_Survelliance_Start(void){
		controller.survelliance_ready = true;
}

void OBC_Survelliance_Stop(void){
		controller.survelliance_ready = false;
}


void ema_callback(){
	if(controller.count_ema > 0){
		Read1553Data(device_params[EMA_Ready], MSG_EMA, controller.EMA_FP);
		if((controller.count_ema % 10 ) == 0)
			send1553FlatBufferObject(EMA_Ready, 0);
		controller.count_ema--;
	}
	else{
		timerTh->stop_timer(&controller.t_ema);
		close(controller.EMA_FP);
		LogwithTimestamp((char *)" EMA STOPPED ");
	}
}


void EMA_Start(void){
	controller.ema_ready = true;

	time_t t = std::time(0);   // get time now
	tm* now = std::localtime(&t);

	char log_file[64];

	int count = sprintf(log_file,"../out/EMA-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';

	cout << "log file name " << log_file << endl;

	unlink(EMA_FILE_NAME);
	count = symlink(log_file, EMA_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", LOG_FILE_NAME);
	}

	controller.EMA_FP = open(EMA_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);

	if(controller.EMA_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	cout << "frequency " << device_params[EMA_Ready]->frequency << endl;

	if ( device_params[EMA_Ready]->frequency == -1){
		controller.count_ema = ULONG_MAX;
	      	
	} 
	else{
		controller.count_ema = device_params[EMA_Ready]->frequency * 100;
	}

	timerTh->start_timer(&controller.t_ema, 0, 10, ema_callback);
	LogwithTimestamp((char *)" EMA LOG STARTED ");

}


void lev_callback(){

	if( controller.count_ins > 0){
		Read1553Data(device_params[INS_Ready], MSG_INS_LEV, controller.LEV_LOG_FP);
		send1553FlatBufferObject(INS_Ready, 0);
		controller.count_ins--;
        
	}
	else{
		timerTh->stop_timer(&controller.t_lev);
		LogwithTimestamp((char *)"LEVING OVER");
	}
}


void INS_Data_Start(void){
	controller.ins_ready = true;

	char log_file[64];

	time_t t = std::time(0);   // get time now
	tm* now = std::localtime(&t);


	int count = sprintf(log_file,"../out/LEV-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(LEV_LOG_NAME);
	count = symlink(log_file, LEV_LOG_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", LEV_LOG_NAME);
	}

	controller.LEV_LOG_FP = open(LEV_LOG_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(controller.LEV_LOG_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	controller.count_ins = device_params[INS_Ready]->frequency * 50;

	//fprintf(stdout, " counnter -- %ld frequnecy - %d \n", controller.count_ins, device_params[INS_Ready]->frequency);

	timerTh->start_timer(&controller.t_lev, 0, 20, lev_callback);
	device_params[INS_Ready]->direction = 1;
	LogwithTimestamp((char *)"LEVELLING STARTED ");

}


void Nav_callback(void *){
	static int counter =0 ;
	counter++;

	if(controller.count_ins > 0){

		Read1553Data(device_params[17], MSG_INS_17, controller.NAV_17_FP);
		send1553FlatBufferObject(17, 0);

		if((counter % 2) == 0){
			Read1553Data(device_params[18], MSG_INS_18, controller.NAV_18_FP);
			send1553FlatBufferObject(18, 0);
		}
		controller.count_ins--;

	}
	else{

		timerTh->stop_timer(&controller.t_nav);
		printf(" NAV __ STOPEDD \n");
	}
}






void NAV_Data_Start(void){
	controller.count_ins = device_params[NAV_Ready]->frequency * 100;
	timerTh->start_timer(&controller.t_nav, 0, 10, Nav_callback);

}


void TWDL_Start(void){

	controller.twdl_uplink = controller.twdl_downlink = true;
}

void TWDL_Stop(void){

	controller.twdl_uplink = controller.twdl_downlink = false;
}


void Read1553Data(config_params_t *t12, int msg_index, int file_fd){

	unsigned long t1 = 0;
	unsigned char buffer[8+2+64];
	ddc->getUpdatedData(t12, msg_index);
	t1 = gettimestamp();

	memset(buffer, 0, sizeof buffer);
	memcpy(&buffer, &t1, sizeof t1);
	memcpy(&buffer[8], &t12->CommandWord, sizeof t12->CommandWord);
	memcpy(&buffer[10], &t12->payload, sizeof t12->payload);

	write(file_fd, buffer, sizeof buffer);

}


void ReadINSData(void *){

	static unsigned short counter = 0;
	//while(true)
	{

	device_params[INS_Servelliance]->Subaddr = 16;
	Read1553Data(device_params[INS_Servelliance], MSG_INS_SURV, controller.INS_LOG_FP);
	if(controller.ins_survelliance)
		if( (counter % 40 ) == 0)
			send1553FlatBufferObject(INS_Servelliance, 1);
	counter++;
	}


}


void TWDLData(void *){

	int counter = 0;

	//while(true)
	{
		//usleep(100 *1000);

		device_params[TWDL_PktA]->Subaddr = TWDL_PktA;
		Read1553Data(device_params[TWDL_PktA], MSG_TWDL_PktA, controller.TWDL_A_FP);
		send1553FlatBufferObject(TWDL_PktA, 0);

		device_params[TWDL_PktC]->Subaddr = TWDL_PktC;
		Read1553Data(device_params[TWDL_PktC], MSG_TWDL_PktC,controller.TWDL_C_FP);
		send1553FlatBufferObject(TWDL_PktC, 0);

		counter++;
	}


}



void ReadSurvellianceData(void*){

	static int counter = 0;
	//while(true)
	{
		//usleep(10 *1000);
		/* Read OBC survellinace Data */
		device_params[8]->Subaddr = 8;
		Read1553Data(device_params[8], MSG_OBC_SURV, controller.SUR_LOG_FP);
		if(counter % 10 == 0)
			send1553FlatBufferObject(8, 1);
		counter++;
	}

}


void *Dispatcher(void*){
	while(true)
	{

		FD_ZERO(& (controller.readfds));
		FD_SET(controller.CMD_PIPE, & (controller.readfds));


		int retstatus = controller.CommandHandler(controller.CMD_PIPE, controller.readfds);

		printf("Size of object=%d \n", retstatus);

		// guarding to avoid the flatbuffer object crash in future it may increase
		if(retstatus > 0 && retstatus < 160)
		{
			getFlatBufferObject(((controller.pipe_data).object));
		}

	}
	return NULL;
}




void GpsRead(void *){
	int file_fp[] = {controller.TWDL_H_FP, controller.TWDL_S_FP, controller.NAV_19_FP, 
					controller.NAV_20_FP, controller.NAV_21_FP, controller.NAV_22_FP};
	unsigned char single_time[] = {MSG_TWDL_Health, MSG_TWDL_Status, MSG_INS_19,
										MSG_INS_20,
										MSG_INS_21,
										MSG_INS_22};
	unsigned char single_sub[] = {12, 13, 19, 20, 21, 22};

	//while(1)
	{
		//sleep(1);
		for(unsigned long i=0; i<sizeof single_time; i++){
			device_params[single_sub[i]]->Subaddr = single_sub[i];
			Read1553Data(device_params[single_sub[i]], single_time[i], file_fp[i]);
			send1553FlatBufferObject(single_sub[i], 0);
			usleep(1000);
		}
		
	}

}



void SeekerRead(void *){
	static int counter = 0;
	//while(1)
	{
		//usleep(20 * 1000);

		device_params[28]->Subaddr = 28;
		Read1553Data(device_params[28], MSG_28, controller.SeekerA_FP);
		if(counter % 5 == 0)
			send1553FlatBufferObject(28, 0);

		//usleep(1000);

		device_params[29]->Subaddr = 29;
		Read1553Data(device_params[29], MSG_29, controller.SeekerB_FP);
		if(counter % 5 == 0)
			send1553FlatBufferObject(29, 0);
		counter++;
	}

}


/* Exit Handler */
void Handle_CtrlC(int sig){

	fprintf(stdout, " at closing handler of main application with signal -- %d \n", sig);

#if 1
	unlink(controller.DevConfig.cmd_pipe.c_str());
	unlink(controller.DevConfig.rsp_pipe.c_str());
	unlink(controller.DevConfig.sur_pipe.c_str());
	unlink(controller.DevConfig.err_pipe.c_str());
	unlink(controller.DevConfig.adc_pipe.c_str());
	unlink(controller.DevConfig.ema_pipe.c_str());
#endif

	if(controller.LOG_FP){
		close(controller.LOG_FP);
	}
	if(controller.INS_LOG_FP){
		close(controller.INS_LOG_FP);
	}
	if(controller.EMA_FP){
		close(controller.EMA_FP);
	}
	if(controller.SUR_LOG_FP)
		close(controller.SUR_LOG_FP);
	if(controller.SeekerA_FP)
		close(controller.SeekerA_FP);
	if(controller.SeekerB_FP)
		close(controller.SeekerB_FP);
	if(controller.ADC_FP)
		close(controller.ADC_FP);


	pthread_cancel(controller.threadId);

	if(ddc_result != FAILED){
#if 0
		pthread_cancel(controller.readId);
		pthread_cancel(controller.twdlId);
		pthread_cancel(controller.GpsId);
		pthread_cancel(controller.seekerId);
		pthread_cancel(controller.insId);
#else
		timerTh->stop_timer(&controller.t_ins);
		timerTh->stop_timer(&controller.t_seeker);
		timerTh->stop_timer(&controller.t_onesec);
		timerTh->stop_timer(&controller.t_obc_surv);
		timerTh->stop_timer(&controller.t_ema);
		timerTh->stop_timer(&controller.t_nav);
		timerTh->stop_timer(&controller.t_lev);

#endif
		delete	ddc;
	}

	timerTh->stop_timer(&controller.t_adc);
	delete timerTh;
	_exit(0);
}


void adc_read(){
	//printf(" ADC Read \n");

	int ret = GetData(&controller.adc_channel);
	if(ret != 0){

	}
	else{
		write(controller.ADC_PIPE, &controller.adc_channel.value, sizeof controller.adc_channel.value);
	}

	unsigned char buffer[ 8 + 8];
	unsigned long t1 = gettimestamp();
	memcpy(&buffer, &t1, sizeof t1);
	memcpy(&buffer[8], &controller.adc_channel.value, sizeof controller.adc_channel.value);
	write(controller.ADC_FP, buffer, sizeof buffer);
}



void Controller::Initialize_Data_Pipes(void){

	if(access( controller.DevConfig.cmd_pipe.c_str(),F_OK&(R_OK|W_OK))){
		if (mkfifo( controller.DevConfig.cmd_pipe.c_str(),0600) ){
			printf("Unable to Create %s FIFO \n",controller.DevConfig.cmd_pipe.c_str());
		}
	
	}
	controller.CMD_PIPE = open( controller.DevConfig.cmd_pipe.c_str(), O_RDWR|O_NONBLOCK);
		if(controller.CMD_PIPE < 0 ){
		printf(" %s  PIPE not opened \n",controller.DevConfig.cmd_pipe.c_str());
		}
	else
	printf(" CMD_PIPE == %d \n",controller.CMD_PIPE);



	if(access( controller.DevConfig.rsp_pipe.c_str(),R_OK|W_OK)){
		if(mkfifo( controller.DevConfig.rsp_pipe.c_str(),0600) ){
			printf("Unable to Create %s FIFO \n",controller.DevConfig.rsp_pipe.c_str());
		}	
	}
	controller.RSP_PIPE = open( controller.DevConfig.rsp_pipe.c_str(),O_RDWR|O_NONBLOCK);
	if(controller.RSP_PIPE < 0 ){
		printf(" %s  PIPE not opened \n",controller.DevConfig.rsp_pipe.c_str());
	}
	else
		printf(" RSP_PIPE == %d \n",controller.RSP_PIPE);

	if(access( controller.DevConfig.sur_pipe.c_str(),R_OK|W_OK)){
		if(mkfifo( controller.DevConfig.sur_pipe.c_str(),0600) ){
			printf("Unable to Create %s FIFO \n",controller.DevConfig.sur_pipe.c_str());
		}	
	}
	controller.SUR_PIPE = open( controller.DevConfig.sur_pipe.c_str(),O_RDWR|O_NONBLOCK);
	if(controller.SUR_PIPE < 0 ){
		printf(" %s  PIPE not opened \n",controller.DevConfig.sur_pipe.c_str());
	}
	else
		printf(" SUR_PIPE == %d \n",controller.SUR_PIPE);

	if(access( controller.DevConfig.err_pipe.c_str(),R_OK|W_OK)){
		if(mkfifo( controller.DevConfig.err_pipe.c_str(),0600) ){
			printf("Unable to Create %s FIFO \n",controller.DevConfig.err_pipe.c_str());
		}	
	}
	controller.ERR_PIPE = open( controller.DevConfig.err_pipe.c_str(),O_RDWR|O_NONBLOCK);
	if(controller.ERR_PIPE < 0 ){
		printf(" %s  PIPE not opened \n",controller.DevConfig.err_pipe.c_str());
	}
	else
		printf(" ERR_PIPE == %d \n",controller.ERR_PIPE);


	if(access( controller.DevConfig.adc_pipe.c_str(),R_OK|W_OK)){
		if(mkfifo( controller.DevConfig.adc_pipe.c_str(),0600) ){
			printf("Unable to Create %s FIFO \n",controller.DevConfig.adc_pipe.c_str());
		}
	}
	controller.ADC_PIPE = open( controller.DevConfig.adc_pipe.c_str(),O_RDWR|O_NONBLOCK);
	if(controller.ADC_PIPE < 0 ){
			printf(" %s  PIPE not opened \n",controller.DevConfig.adc_pipe.c_str());
	}
	else
		printf(" ADC_PIPE == %d \n",controller.ADC_PIPE);


	if(access( controller.DevConfig.ema_pipe.c_str(),R_OK|W_OK)){
		if(mkfifo( controller.DevConfig.ema_pipe.c_str(),0600) ){
			printf("Unable to Create %s FIFO \n",controller.DevConfig.ema_pipe.c_str());
		}
	}
	controller.EMA_PIPE = open( controller.DevConfig.ema_pipe.c_str(),O_RDWR|O_NONBLOCK);
	if(controller.EMA_PIPE < 0 ){
			printf(" %s  PIPE not opened \n",controller.DevConfig.ema_pipe.c_str());
	}
	else
		printf(" EMA_PIPE == %d \n",controller.EMA_PIPE);

}


void Controller::GetConfigParams(){

	   // Read the file. If there is an error, report it and exit.
		try{
			cfg.readFile(DEV_CONFIG_FILE);
		}
		catch(const FileIOException &fioex)	{
			cerr << "I/O error while reading file."  << fioex.what() << endl;
			return ;
		}
		catch(const ParseException &pex)	{
				cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
						  << " - " << pex.getError() << endl;
				return;
		}

		const Setting& root = cfg.getRoot();

		try{

			const Setting &Interface_root = root["dev_config"];

			const Setting &Interface_list = Interface_root["Interface_list"];

			Interface_root.lookupValue("Make", DevConfig.Dev_Make);
			cout << "Make " <<  DevConfig.Dev_Make << endl;

			Interface_root.lookupValue("Interface_Type", DevConfig.Dev_Interface);
			cout << "Interface Type " <<  DevConfig.Dev_Interface << endl;

			const Setting &dev_Params = Interface_root[Interface_list[0].c_str()];

			dev_Params.lookupValue("ModuleNum", DevConfig.modulenum);
			dev_Params.lookupValue("DeviceNum", DevConfig.devicenum);

			cout << " Module " << DevConfig.modulenum << endl;
			cout << " Device " <<  DevConfig.devicenum << endl;

			Interface_root.lookupValue("cmd_pipe", DevConfig.cmd_pipe);
			cout << " cmd_pipe " <<  DevConfig.cmd_pipe << endl;

			Interface_root.lookupValue("rsp_pipe", DevConfig.rsp_pipe);
			cout << " rsp_pipe " <<  DevConfig.rsp_pipe << endl;

			Interface_root.lookupValue("err_pipe", DevConfig.err_pipe);
			cout << " err_pipe " <<  DevConfig.err_pipe << endl;

			Interface_root.lookupValue("sur_pipe",DevConfig.sur_pipe);
			cout << "sur_pipe " << DevConfig.sur_pipe <<endl;

			Interface_root.lookupValue("lock_filename",DevConfig.lock_filename);
			cout << "lock_filename" << DevConfig.lock_filename << endl;

			Interface_root.lookupValue("description", DevConfig.description);
			cout << "description  " << DevConfig.description << endl;

			Interface_root.lookupValue("adc_pipe", DevConfig.adc_pipe);
			cout << "adc pipe "<< DevConfig.adc_pipe;

			Interface_root.lookupValue("ema_pipe", DevConfig.ema_pipe);
			cout << "ema pipe "<< DevConfig.ema_pipe;

		}
		catch(const SettingNotFoundException &nfex){
			  cout << "Setting Not Found" << nfex.what() << endl;
		}
}

int Controller::CommandHandler(int fd, fd_set readfds){
	int status=FAILED;
	while(1)
	{
		status=select(fd+1, &readfds, NULL, NULL, NULL);
		if(status == TIMEDOUT)
		{
			printf("##################Time out waiting################ %d\n", status);
		}
		if(status == DATA_READY )
		{
			memset (&(controller.pipe_data), 0x00, sizeof( controller.pipe_data));
			int rcvd_wordcount = read(fd, &(controller.pipe_data), sizeof( controller.pipe_data));
			cout<<"-----------------  Command Packet Received  size----------------" << rcvd_wordcount<<endl;
			return rcvd_wordcount;
		}

		return status;
	}
}



void Controller::Create_log_files(void){

	char log_file[64];
	time_t t = std::time(0);   // get time now
	tm* now = std::localtime(&t);

	int count = sprintf(log_file,"../out/log-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(LOG_FILE_NAME);
	count = symlink(log_file, LOG_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", LOG_FILE_NAME);
	}

	LOG_FP = open(LOG_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(LOG_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	count = sprintf(log_file,"../out/ins-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(INS_LOG_NAME);
	count = symlink(log_file, INS_LOG_NAME);
	if(count){
		fprintf(stderr, " Link failure for %s \n", INS_LOG_NAME);
	}
	INS_LOG_FP = open(INS_LOG_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(INS_LOG_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}


	count = sprintf(log_file,"../out/twdl-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(TWDL_LOG_NAME);
	count = symlink(log_file, TWDL_LOG_NAME);
	if(count){
		fprintf(stderr, " Link failure for %s \n", TWDL_LOG_NAME);
	}
	TWDL_LOG_FP = open(TWDL_LOG_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(TWDL_LOG_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	count = sprintf(log_file,"../out/surv-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(SUR_LOG_NAME);
	count = symlink(log_file, SUR_LOG_NAME);
	if(count){
		fprintf(stderr, " Link failure for %s \n", SUR_LOG_NAME);
	}
	SUR_LOG_FP = open(SUR_LOG_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(TWDL_LOG_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

#if 1
	count = sprintf(log_file,"../out/nav-19-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(NAV_19_FILE_NAME);
	count = symlink(log_file, NAV_19_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", NAV_19_FILE_NAME);
	}

	NAV_19_FP = open(NAV_19_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(NAV_19_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	count = sprintf(log_file,"../out/nav-20-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(NAV_20_FILE_NAME);
	count = symlink(log_file, NAV_20_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", NAV_20_FILE_NAME);
	}

	NAV_20_FP = open(NAV_20_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(NAV_20_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}


	count = sprintf(log_file,"../out/nav-21-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(NAV_21_FILE_NAME);
	count = symlink(log_file, NAV_21_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", NAV_21_FILE_NAME);
	}

	NAV_21_FP = open(NAV_21_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(NAV_21_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	count = sprintf(log_file,"../out/nav-22-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(NAV_22_FILE_NAME);
	count = symlink(log_file, NAV_22_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", NAV_22_FILE_NAME);
	}

	NAV_22_FP = open(NAV_22_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(NAV_22_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}



	count = sprintf(log_file,"../out/nav-17-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(NAV_17_FILE_NAME);
	count = symlink(log_file, NAV_17_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", NAV_17_FILE_NAME);
	}

	NAV_17_FP = open(NAV_17_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(NAV_17_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	
	count = sprintf(log_file,"../out/nav-18-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(NAV_18_FILE_NAME);
	count = symlink(log_file, NAV_18_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", NAV_18_FILE_NAME);
	}

	NAV_18_FP = open(NAV_18_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(NAV_18_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}



	count = sprintf(log_file,"../out/twdl-A-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(TWDL_A_FILE_NAME);
	count = symlink(log_file, TWDL_A_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", TWDL_A_FILE_NAME);
	}

	TWDL_A_FP = open(TWDL_A_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(TWDL_A_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	
	count = sprintf(log_file,"../out/twdl-status-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(TWDL_STATUS_FILE_NAME);
	count = symlink(log_file, TWDL_STATUS_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", TWDL_STATUS_FILE_NAME);
	}

	TWDL_S_FP = open(TWDL_STATUS_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(TWDL_S_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}

	count = sprintf(log_file,"../out/twdl-health-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(TWDL_HEALTH_FILE_NAME);
	count = symlink(log_file, TWDL_HEALTH_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", TWDL_HEALTH_FILE_NAME);
	}

	TWDL_H_FP = open(TWDL_HEALTH_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(TWDL_H_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}


	count = sprintf(log_file,"../out/twdl-C-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;

	unlink(TWDL_C_FILE_NAME);
	count = symlink(log_file, TWDL_C_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", TWDL_C_FILE_NAME);
	}

	TWDL_C_FP = open(TWDL_C_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(TWDL_C_FP == -1){
		cout << "log file creation error->"<< log_file << endl;
	}


	count = sprintf(log_file,"../out/Seeker-A-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;
	unlink(SEEKER_A_FILE_NAME);
	count = symlink(log_file, SEEKER_A_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", SEEKER_A_FILE_NAME);
	}
	SeekerA_FP = open(SEEKER_A_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(SeekerA_FP == -1){
			cout << "log file creation error->"<< log_file << endl;
		}


	count = sprintf(log_file,"../out/Seeker-B-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;
	unlink(SEEKER_B_FILE_NAME);
	count = symlink(log_file, SEEKER_B_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", SEEKER_B_FILE_NAME);
	}
	SeekerB_FP = open(SEEKER_B_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(SeekerB_FP == -1){
			cout << "log file creation error->"<< log_file << endl;
		}


	count = sprintf(log_file,"../out/ADC-%02d-%02d%02d%02d.bin", now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	log_file[count] = '\0';
	cout << "log file name " << log_file << endl;
	unlink(ADC_FILE_NAME);
	count = symlink(log_file, ADC_FILE_NAME);
	if(count){
		fprintf(stdout, " Link falilure for %s \n", SEEKER_B_FILE_NAME);
	}
	ADC_FP = open(ADC_FILE_NAME, O_RDWR|O_CREAT|O_NONBLOCK, S_IRWXU|S_IRWXG|S_IRWXO);
	if(ADC_FP == -1){
			cout << "log file creation error->"<< log_file << endl;
		}

#endif


}


Controller::Controller(){

	CMD_PIPE = FAILURE;
	RSP_PIPE = FAILURE;
	SUR_PIPE = FAILURE;
	ERR_PIPE = FAILURE;
	ADC_PIPE = FAILURE;
	EMA_PIPE = FAILURE;

	threadId = FAILURE;
	readId = FAILURE;
	emaId = FAILURE;
	twdlId = surId = insId = NavId = GpsId = seekerId = FAILURE;

	LOG_FP = FAILURE;
	INS_LOG_FP = FAILURE;
	EMA_FP = NAV_LOG_FP = LEV_LOG_FP = SUR_LOG_FP = TWDL_LOG_FP = SeekerA_FP = SeekerB_FP = FAILURE;

	survelliance_ready = false;
	normal_data = false;
	ema_ready = false;
	ins_ready = false;
	nav_ready = false;

	ins_survelliance = true;
	t_ins = t_ema = t_obc_surv = t_twdl = t_seeker = t_onesec = t_lev = t_nav = 0;
	count_ema = count_ins = 0;
	demand_subaddress = 0;
	freq = 0;
	total_samples = 0;
	rc  = 0;

	twdl_uplink =  twdl_downlink = false;

	NAV_17_FP = NAV_18_FP = NAV_19_FP = NAV_20_FP = NAV_21_FP = NAV_22_FP = -1;
	TWDL_A_FP = TWDL_C_FP = TWDL_H_FP = TWDL_S_FP = ADC_FP = -1;

	t_ins = t_obc_surv = t_seeker = t_onesec = t_twdl = NULL;
	t_adc = NULL;

	FunctionPointer[0] = &ReceiveCommand;
	FunctionPointer[1] = &TransmitCommand;

	FunctionPointer[INS_Servelliance] = &INS_Survelliance_Start; //INS_Data_Start;
	FunctionPointer[OBC_Servelliance] = &OBC_Survelliance_Start;

	FunctionPointer[EMA_Ready] = &EMA_Start;
	FunctionPointer[INS_Ready] = &INS_Data_Start;
	FunctionPointer[NAV_Ready] = &NAV_Data_Start;

}

int main( int argc, char **argv){

	controller.GetConfigParams();
	controller.Initialize_Data_Pipes();
	controller.Create_log_files();

	/* handle the ctrl+c operation*/
	signal(SIGINT, Handle_CtrlC);

	//signal(SIGALRM, callback);

	//Upgarde to super user
	seteuid(0);

	timerTh = new TimerThread();


	dop=DOP::getDopInstance();
	int result = dop->initDo();
	if(result==FAILED)
	{
		write(controller.ERR_PIPE, ErrDopInit,  strlen(ErrDopInit));
	}

	dip=DIP::getDipInstance();
	result = dip->initDI();
	if(result==FAILED)
	{
		write(controller.ERR_PIPE, ErrDipInit,  strlen(ErrDipInit));	
	}
	
	adc=ADC::getADCInstance();	
	result = adc->init_ADC_CARD();	
	if(result==FAILED)
	{
		write(controller.ERR_PIPE, ErrAdcInit,  strlen(ErrAdcInit));
	}



	rc = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(rc != 0){
		errExitEN(rc, "pthread_setcanceltype");
	}


	ddc=DDC1553::getDDC1553Instance();
	ddc_result = ddc->initDDC();
	if(ddc_result == FAILED)
	{
		write(controller.ERR_PIPE, ErrDdcInit,  strlen(ErrDdcInit));	
	}
	else
	{

		for(int kk =0; kk<32; kk++){
			device_params[kk] = (config_params_t *)calloc(sizeof(config_params_t), 1);
			device_params[kk]->Rtnum = 1;
			device_params[kk]->Subaddr = kk;
			device_params[kk]->DataBlk = 250 + kk;
			device_params[kk]->MsgID = 500 + kk;
		}

#if 0
		rc = pthread_create(&controller.readId, NULL, ReadSurvellianceData, NULL);
		if (rc != 0)
			   errExitEN(rc, "pthread_create,ReadSurvData");

		rc = pthread_setname_np(controller.readId, "ReadSurvData");
		if (rc != 0)
			   errExitEN(rc, "pthread_getname_np, ReadSurvData");

		rc = pthread_create(&controller.insId, NULL, ReadINSData, NULL);
		if (rc != 0)
			   errExitEN(rc, "pthread_create,ReadINSData");

		rc = pthread_setname_np(controller.insId, "ReadINSData");
		if (rc != 0)
			   errExitEN(rc, "pthread_getname_np,ReadINSData ");

		rc = pthread_create(&controller.twdlId, NULL, TWDLData, NULL);
		if (rc != 0)
			   errExitEN(rc, "pthread_create, TWDLData");

		rc = pthread_setname_np(controller.twdlId, "TWDLData");
		if (rc != 0)
			   errExitEN(rc, "pthread_getname_np, TWDLData");
		
		// 1 sec thread
		rc = pthread_create(&controller.GpsId, NULL, GpsRead , NULL);
		if (rc != 0)
			   errExitEN(rc, "pthread_create, GpsRead");

		rc = pthread_setname_np(controller.GpsId, "GpsRead");
		if (rc != 0)
		   errExitEN(rc, "pthread_getname_np, GpsRead");
		
		// 20 msec

		rc = pthread_create(&controller.seekerId, NULL, SeekerRead, NULL);
		if (rc != 0)
			   errExitEN(rc, "pthread_create, SeekerRead");

		rc = pthread_setname_np(controller.GpsId, "SeekerRead");
		if (rc != 0)
		   errExitEN(rc, "pthread_getname_np, SeekerRead");
#else
		timerTh->start_timer(&controller.t_ins, 0, 2.5, ReadINSData);
		timerTh->start_timer(&controller.t_obc_surv, 0, 10, ReadSurvellianceData);
		timerTh->start_timer(&controller.t_seeker, 0, 20, SeekerRead);
		timerTh->start_timer(&controller.t_twdl, 0, 100, TWDLData);
		timerTh->start_timer(&controller.t_onesec, 1, 0, GpsRead);

#endif

	}

	rc = pthread_create(&controller.threadId, NULL, Dispatcher, NULL);
	if (rc != 0)
		errExitEN(rc, "pthread_create,Dispatcher");

	rc = pthread_setname_np(controller.threadId, "Dispatcher");
	if (rc != 0)
		errExitEN(rc, "pthread_getname_np, Dispatcher");


	Init_AI(&controller.DevConfig);

	timerTh->start_timer(&controller.t_adc, 0, 100, adc_read);


#if 0
	// first logic should be non -preemptive then we can focus on the affinity scheduling
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);

	CPU_SET(0, &cpuset);
	CPU_SET(1, &cpuset);

	rc = pthread_setaffinity_np(controller.readId, sizeof(cpu_set_t), &cpuset);
	if (rc != 0)
		   errExitEN(rc, "pthread_setaffinity_np");
#endif

	if(controller.threadId != NULL)
		pthread_join(controller.threadId, NULL);

#if 0
	if(controller.seekerId != NULL)
		pthread_join(controller.seekerId, NULL);
	if(controller.GpsId != NULL)
		pthread_join(controller.GpsId, NULL);
	if(controller.twdlId != NULL)
		pthread_join(controller.twdlId, NULL);
	if(controller.insId != NULL)
		pthread_join(controller.insId, NULL);
	if(controller.readId != NULL)
		pthread_join(controller.readId, NULL);
#endif
	

	return 0;
}
