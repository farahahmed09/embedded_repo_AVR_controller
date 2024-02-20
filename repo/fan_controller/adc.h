
#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   2.56



typedef enum{
	AREF,AVCC,reserved,INTERNAL_VOLT
}ADC_ReferenceVoltage;

typedef enum{
	FCPU_div_2=1,FCPU_div_4,FCPU_div_8,FCPU_div_16,FCPU_div_32,FCPU_div_64,FCPU_div_128
}ADC_Prescaler;

typedef struct{
	ADC_ReferenceVoltage ref_volt ;
	ADC_Prescaler prescaler;
}ADC_ConfigType;




/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init( ADC_ConfigType * Config_Ptr);
/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */
uint16 ADC_readChannel(uint8 channel_num);

#endif /* ADC_H_ */
