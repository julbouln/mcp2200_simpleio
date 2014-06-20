#include <mcp2200_simpleio.h>
#include <iostream>
#include <bitset>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
    
using namespace SimpleIO;	
	
void print_help(void)
{
	cout << "mcp2200_config - MCP2200 configuration tool\n\n";
	cout << "USAGE: mcp2200_config options\n\n";
	cout << "OPTIONS:\n";
	cout << " --show\tShow current configuration\n";
	cout << " --vendorId <ID>\tLook for vendorID\n";					 
	cout << " --productId <ID>\tLook for productID\n";
	cout << " --baud <rate>\tSet baud rate\n";
	cout << " --rx_led <mode>\tSet RX LED mode\n";
	cout << " \t\t\tAvailable modes:\n";
	cout << " \t\t\t0: Led is disabled\n";
	cout << " \t\t\t3: Toggle LED\n";
	cout << " \t\t\t4: Blink LED (200 ms)\n";
	cout << " \t\t\t5: Blink LED (100 ms)\n";
	cout << " --tx_led <mode>\tSet TX LED mode\n";
	cout << " --cts_rts <bool>\tEnable/Disable CTS/RTS pins\n";
	cout << " --usbcfg <bool>\tEnable/Disable USBCFG pin\n";
	cout << " --suspend <bool>\tEnable/Disable Suspend pin\n";
	cout << " --manufacturer <string>\tSet manufacturer string\n";
	cout << " --io_dir <bin>\tSet default pins direction (input 0, output 1)\n";
	cout << " --io_val <bin>\tSet default pins output values\n";
	cout << " --product <string>\tSet product string\n";
	cout << " --help\tThis help\n";
}	
	
int main(int argc, char** argv) {
	int vendorID=0x04D8;
	int productID=0x00DF;
	int rx_led=-1;
	int tx_led=-1;
	int cts_rts=-1;
	int usbcfg=-1;
	int suspend=-1;
	int invert_pol=-1;
	int io_val=-1;
	int io_dir=-1;

	char *manufacturer=NULL;
	char *product=NULL;

	int baud=-1;
	int show=0;
	int c;
     
	while (1)
	{
		static struct option long_options[] =
		{
			{"baud",  required_argument,       0, 0},
			{"rx_led",     required_argument,       0, 0},
			{"tx_led",     required_argument,       0, 0},				   
			{"cts_rts",     required_argument,       0, 0},
			{"usbcfg",     required_argument,       0, 0},
			{"suspend",     required_argument,       0, 0},
			{"invert_pol",     required_argument,       0, 0},

			{"manufacturer",  required_argument,       0, 0},
			{"product",  required_argument,       0, 0},

			{"io_dir", required_argument,       0, 0},
			{"io_val", required_argument,       0, 0},

			{"show", no_argument,       0, 's'},
			{"vendorId",  required_argument, 0, 'v'},
			{"productId",  required_argument, 0, 'p'},
			{"help",    no_argument, 0, 'h'},
				   
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;
     
		c = getopt_long (argc, argv, "shv:p:",
		long_options, &option_index);
     
		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
			case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
				   
			switch(option_index) {
				case 0:
				baud=strtol(optarg, NULL, 10);
				break;
				case 1:
				rx_led=strtol(optarg, NULL, 10);
				break;
				case 2:
				tx_led=strtol(optarg, NULL, 10);
				break;
				case 3:
				cts_rts=strtol(optarg, NULL, 10);
				break;
				case 4:
				usbcfg=strtol(optarg, NULL, 10);
				break;
				case 5:
				suspend=strtol(optarg, NULL, 10);
				break;
				case 6:
				invert_pol=strtol(optarg, NULL, 10);
				break;					   
				case 7:
				manufacturer=optarg;
				break;
				case 8:
				product=optarg;
				break;
				case 9:
				io_dir=strtol(optarg,NULL,2);
				break;
				case 10:
				io_val=strtol(optarg,NULL,2);
				break;

			}
			break;
			case 'h':
			// help
			print_help();
			exit(0);
			break;
			case 'v':
			vendorID=strtol(optarg, NULL, 16);
			break;     
			case 'p':
			productID=strtol(optarg, NULL, 16);
			break;
			case 's':
			show=1;
			break;
			default:
			print_help();
			exit(0);
		}
	}
          
	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
	}
	if(argc==1) {
		print_help();
		exit(0);
	}
     
	SimpleIOClass::InitMCP2200(vendorID, productID);

	if(SimpleIOClass::IsConnected()) {
		printf("Found MCP2200 0x%04X:0x%04X\n",vendorID,productID);
		if(baud>-1) {
			cout << "Set baud rate to " << baud << "\n";
			SimpleIOClass::fnSetBaudRate(baud);
		}
		if(tx_led>-1) {
			cout << "Set TX LED mode to " << tx_led << "\n";
			SimpleIOClass::fnTxLED(tx_led);
		  
		}
		if(rx_led>-1) {
			cout << "Set RX LED mode to " << rx_led << "\n";
			SimpleIOClass::fnRxLED(rx_led);
		}
		
		if(cts_rts>-1) {
			cout << "Set CTS/RTS to " << cts_rts << "\n";
			SimpleIOClass::fnHardwareFlowControl(cts_rts);
		}
		if(usbcfg>-1) {
			cout << "Set USBCFG to " << usbcfg << "\n";
			SimpleIOClass::fnULoad(usbcfg);
		}
		if(suspend>-1) {
			cout << "Set Suspend to " << suspend << "\n";
			SimpleIOClass::fnSuspend(suspend);
		}
		if(invert_pol>-1) {
			cout << "Set invert UART polarity to " << invert_pol << "\n";
			// TODO
		}
		
		if(manufacturer) {
			cout << "Set manufacturer string to " << manufacturer << "\n";

			SimpleIOClass::SetManufacturerString(manufacturer);
		}
		if(product) {
			cout << "Set product string to " << product << "\n";

			SimpleIOClass::SetProductString(product);
		}
		
		if(io_dir>-1) {			
			string binary = bitset<8>(io_dir).to_string(); 
			cout << "Set IO direction to "<<binary<<"\n";
			SimpleIOClass::ConfigureIO(io_dir);						
		}

		if(io_val>-1) {
			string binary = bitset<8>(io_val).to_string(); 
			cout << "Set IO value to "<<binary<<"\n";
			SimpleIOClass::ConfigureIoDefaultOutput(SimpleIOClass::GetIO(),io_val);
		}
		
		if(show) {
			int currentBaud=SimpleIOClass::GetBaudRate();
			
			string io_binary = bitset<8>(SimpleIOClass::GetIO()).to_string(); 
			string default_output_binary = bitset<8>(SimpleIOClass::GetDefaultOutput()).to_string(); 
			
			cout << "Manufacturer: \t" << SimpleIOClass::GetManufacturerString() << "\n";
			cout << "Product: \t" << SimpleIOClass::SimpleIOClass::GetProductString() << "\n";
			cout << "Baud rate: \t" << currentBaud << "\n";
			cout << "RX LED mode: \t" << SimpleIOClass::GetRxLED() << "\n";
			cout << "TX LED mode: \t" << SimpleIOClass::GetTxLED() << "\n";
			cout << "CTS/RTS pins enabled: \t" << SimpleIOClass::GetHardwareFlowControl() << "\n";
			cout << "USBCFG pin enabled: \t" << SimpleIOClass::GetULoad() << "\n";
			cout << "Suspend pin enabled: \t" << SimpleIOClass::GetSuspend() << "\n";
			cout << "IO Config: \t" << io_binary << "\n";
			cout << "Ouptut default: \t" << default_output_binary << "\n";
			
		}	    
	}
	else
	{
		cout << "No device found" << "\n";
	}
	exit(0);
}