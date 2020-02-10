
#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "Headers/GyroAcc.h"
#include "Headers/UDP.h"
#include "Headers/tools.h"

uint8_t* master_rec_data = 0;
uint8_t acceleration_register[4];
uint8_t gyro_register[4];

#define ONEUSDELAY 145

static struct netif server_netif;
struct netif *echo_netif;

int main(void)
{


  DAVE_STATUS_t status;

  cycledelay((uint32_t)(ONEUSDELAY * 2));

  status = DAVE_Init();           /* Initialization of DAVE APPs  */
  bmi055_start();

  udp_initialize();

  pakketje test;
  test.packetA = 'H';
  test.packetB = 'A';
  test.packetC = 'L';
  test.packetD = 'L';
  test.packetE = 'O';
  test.packetF = '!';
  test.packetG = '!';
  test.packetH = '!';

  if(status != DAVE_STATUS_SUCCESS)
  {
    XMC_DEBUG("DAVE APPs initialization failed\n");
    while(1U)
    {
    }
  }

  //uint8_t gyro = readGyro(0x00);
  //uint8_t acc = readAcc(0x00);

  while(1U)
  {
	  err_t error;
	  cycledelay((uint32_t)(ONEUSDELAY * 50000));
	  error = udp_printStruct(&test);
  }
}





