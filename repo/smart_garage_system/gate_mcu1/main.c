/*
 * main.c
 *
 *  Created on: Nov 28, 2024
 *      Author: Farah Ahmed
 */

#include "main.h"


/************************************************************************************************
 * 									Global Variables definition
 ************************************************************************************************/
enum_states state = IDLE;
sint8 valid=-1;
uint8 id_buffer[4]={0};
uint8 user_index=0;
uint8 slot1=0;
uint8 slot2=0;
uint8 chosen_slot=0;

int main()
{
   /* init the system */
	InitSystem();

	while(1)
	{
		SystemFlow(state);
	}
	return 0;
}

/********************************************************************************************
 * 									functions definition
 *******************************************************************************************/
void InitSystem(void)
{
	/* init modules */
	LCD_init();
	DCMOTOR_init();
	PIR_init(&pir3);
	/* initialize the local users */
	str_users arrUsers[NUM_OF_USERS] = {
        {"1234", NOT_ASSIGNED}, // User 0
        {"5678", NOT_ASSIGNED}, // User 1
        {"9101", NOT_ASSIGNED}, // User 2
        {"2351", NOT_ASSIGNED}, // User 3
        {"1352", NOT_ASSIGNED}  // User 4
    };

}

void SystemFlow(enum_states state)
{
	switch(state)
		{
		case IDLE :
			InitSystem();
			state = ACTION;
			break;
		case ACTION :
			Display();
			EntryOrExit();
			break;
		case ENTER_ID :
			Display();
			EnterID(id_buffer);
			state = CHECK_ID;
			break;
		case CHECK_ID :
			valid = checkUserID(id_buffer);
			if(valid!=-1)
			{
				Display();
				_delay_ms(5000);
				/* matched */
				user_index= valid;
				state = CHOOSE_SLOT;
			}
			else
			{
				/* wrong pass */
				state = ERROR;
			}

			break;
		case CHOOSE_SLOT :
			Display();
			chooseSlot(user_index);
			state = OPEN_THE_GATE;
			break;
		case OPEN_THE_GATE :
			Display();
			/* ashaghale el motor w awel ma el pir sensor ye2ol enaha meshyet
			 * state = CLOSE_THE_GATE
			 */
			DCMOTOR_rotate(CW,100);
			_delay_ms(5000); /* 3shan astana el door yefta7 */
			while(detect_motion(&pir3)!= LOGIC_LOW)
			{
				/* lesa el3 arabya btemshy */
			}
			state = CLOSE_THE_GATE;
			break;
		case CLOSE_THE_GATE :
			Display();
			/*
			 * ashaghal el motor bl 3aks 3shan ye2fel
			 */
			/* 3arabya 3adet khalas */
			/* hashaghal el dc motor bl 3aks 3shan yenazel el door */
			DCMOTOR_rotate(Anti_CW,100);
			_delay_ms(5000); /* 3shan astana el door ye2fel */
			DCMOTOR_rotate(OFF,100);
			break;
		case ERROR :
			Display();
			break;


		}

}

void EnterID(uint8 * arrbuff)
{
	uint8 i;
	uint8 key;
	for(i=0 ; i<4;i++)
	{
		key=KEYPAD_getPressedKey();
		if(key>=0 && key<=9)
		{
			arrbuff[i]=key;
			_delay_ms(400);
			LCD_displayCharacter('*');
		}
		else i--;
	}
	while(KEYPAD_getPressedKey()!=ENTER)
	{
		/*wait till user press enter button */
	}

}
sint8 checkUserID(uint8 *enteredID) {
    for (int i = 0; i < NUM_OF_USERS; i++) {
        if (strcmp((const char *)arrUsers[i].userID,(const char *) enteredID) == 0) {
            return i; // Return the index of the matching user
        }
    }
    return -1; // ID not found
}

void Display(void)
{
	switch(state)
	{
	case IDLE:
		break;
	case ACTION :
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"press 0 for exit");
		LCD_displayStringRowColumn(1,0,"press 1 for entry");
		break;
	case ENTER_ID:
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"enter your id");
		break;
	case CHECK_ID :
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"matched");
		break;
	case CHOOSE_SLOT:
		LCD_clearScreen();
		displayAvailableSlotsOnLCD();
		break;
	case OPEN_THE_GATE :
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"opening the gate!");
		break;
	case CLOSE_THE_GATE :
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"closing the gate!");
		break;
	case ERROR :
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0, "not matched");

		break;
	}
}
void chooseSlot(uint8 user_index )
{
	uint8 chosen_slot = 0;
	chosen_slot = KEYPAD_getPressedKey();
	if(chosen_slot == 1)
	{
		arrUsers[user_index].user_slot_num = ASSIGNED_TO_S1;
	}
	else if (chosen_slot == 2)
	{
		arrUsers[user_index].user_slot_num= ASSIGNED_TO_S2;
	}
	else
	{
		// Handle invalid choice (slot is taken or invalid key pressed)
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Invalid Slot");
		_delay_ms(1000);
	}


}
void EntryOrExit(void)
{
	uint8 choice = 0;
	choice = KEYPAD_getPressedKey();
	if(choice == 0)
	{
		/* exit */
		state = OPEN_THE_GATE;

	}
	else if(choice ==1)
	{
		/* entry */
		state = ENTER_ID ;

	}
	else
	{

	}


}
void displayAvailableSlotsOnLCD() {

    for (int i = 0; i < NUM_OF_USERS; i++)
    {
        if (arrUsers[i].user_slot_num == NOT_ASSIGNED)
        {
        	slot1=0;
        	slot2=0;
        }
        else if(arrUsers[i].user_slot_num == ASSIGNED_TO_S1)
        {
            slot1 = 1;

        }
        else if(arrUsers[i].user_slot_num == ASSIGNED_TO_S2)
        {
            slot2 = 1;
        }
        else
        {

        }
    }
    if(slot1 ==0 && slot2==0)
    {
		LCD_displayStringRowColumn(0,0, "press 1 for slot1");
		LCD_displayStringRowColumn(1,0, "press 2 for slot2");
    }
    else if(slot1 ==1 && slot2==0)
    {
    	LCD_clearScreen();
    	LCD_displayStringRowColumn(0,0, "press 2 for slot2");
    }
    else if(slot1 ==0 && slot2==1)
    {
    	LCD_clearScreen();
    	LCD_displayStringRowColumn(0,0, "press 1 for slot1");
    }
}

