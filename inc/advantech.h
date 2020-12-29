/*
 * advantech.h
 *
 *  Created on: Nov 3, 2020
 *      Author: checkout
 */

#ifndef INC_ADVANTECH_H_
#define INC_ADVANTECH_H_


namespace AIO_global{




  #include <bdaqctrl.h>
  using namespace Automation::BDaq;
  #include <global.h>

#define Error_AI_Read -1
#define Error_AO_Write  -2
#define Error_AO_InitFailed -3
#define Error_AI_InitFailed -4



  typedef struct{
    unsigned short channel;
    double value;
  }AIO_t;


  InstantAiCtrl *AIhandle;
  InstantAoCtrl *AOhandle;
  ErrorCode ret;

  void printError(){
	if(BioFailed(ret))
	{
	  wchar_t enumString[256];
	  AdxEnumToString(L"ErrorCode", (int32)ret, 256, enumString);
	  printf("Some error occurred. And the last error code is 0x%X. [%ls]\n", ret, enumString);
	}
  }

  int  Init_AI(DevConfig_t *device_config){
    AIhandle = InstantAiCtrl::Create();
    DeviceInformation devInfo(device_config->devicenum, ModeWrite);
    ret = AIhandle->setSelectedDevice(devInfo);
    if(ret){
      printError();
      return Error_AI_InitFailed;
    }
    else{
		for (int i = 0; i < AIhandle->getChannels()->getCount(); i++) {
			ret = AIhandle->getChannels()->getItem(i).setValueRange(V_Neg10To10);
			printError();
		}
    	return SUCCESS;
    }
  }

  int Init_AO(DevConfig_t *device_config){
      AOhandle = InstantAoCtrl::Create();
      DeviceInformation devInfo(device_config->devicenum, ModeWrite);
      ret = AOhandle->setSelectedDevice(devInfo);
      if(ret){
		  printError();
		  return Error_AO_InitFailed;
      }
      else{
    	  for (int i = 0; i < AOhandle->getChannels()->getCount(); i++) {
    	  		ret = AOhandle->getChannels()->getItem(i).setValueRange(V_Neg10To10);
    	  		printError();
    	  	}
    	  return  SUCCESS;
      }
    }


  int GetData(AIO_t *params){
    ret = AIhandle->Read(params->channel, params->value);
    if(ret){
		printError();
		return Error_AI_Read;
    }
    else{
    	return SUCCESS;
    }
  }

  int WriteData(AIO_t *params){
    ret = AOhandle->Write(params->channel, params->value);
    if(ret){
	printError();
		return Error_AO_Write;
    }
    else{
    	return SUCCESS;
    }
  }

  void Close(){
    AIhandle->Dispose();
    AOhandle->Dispose();
  }



}//end of namespace





#endif /* INC_ADVANTECH_H_ */
