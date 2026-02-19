#include "ch455g.h"

static iic_bus_typ *hi2c_ch455;


void CH455G_Init(iic_bus_typ *hi2c){
	hi2c_ch455=hi2c;
	CH455_Write(CH455_8SEG_ON);// ¿ªÆôÏÔÊ¾ºÍ¼üÅÌ
}


void CH455_Write( unsigned short cmd )	//Ð´ÃüÁî
{

	IIC_Master_Transmit(hi2c_ch455,((uint8_t)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR, (uint8_t*)&cmd, 1);
}



unsigned char CH455_Read( void )		//¶ÁÈ¡°´¼ü
{
	unsigned char keycode;

	IIC_Master_Receive(hi2c_ch455,(unsigned char)(CH455_GET_KEY>>7)&CH455_I2C_MASK|0x01|CH455_I2C_ADDR, (uint8_t*)&keycode, 1 );
	return keycode;
}


unsigned char Keyboard_proc(void)
{
    unsigned char keyID;
    keyID = CH455_Read();
    if(keyID>67){
        if(keyID<72){
           return (keyID-68)*4+1;
        }else if(keyID<80){
           return (keyID-76)*4+2;
        }else if(keyID<88){
            return (keyID-84)*4+3;
        }else if(keyID<96){
            return (keyID-92)*4+4;
        }
    }
 
    return 0;
}

unsigned char Keyboard_proc_flag=0;
unsigned char Keyboard_proc_f(void){
    unsigned char Keyboard_proc_val;
    Keyboard_proc_val=Keyboard_proc();
    if(Keyboard_proc_val&&Keyboard_proc_flag!=Keyboard_proc_val){
        Keyboard_proc_flag=Keyboard_proc_val;
        return Keyboard_proc_val;
    }else if(Keyboard_proc_val==0){
        Keyboard_proc_flag=0;
    }
    return 0;
}





