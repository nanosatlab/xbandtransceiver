# **Description**

The subsystem is designed as a generic X-band front-end subsystem. It will have to communicate with an On-Board-Computer (OBC) using UART protocol, be powered from the spacecraft’s 12 V power bus and have a PC/104 daughter board form factor. It also has to ensure a downlink data rate of at least 10 Mbps from 5º over the horizon at a 550 km LEO orbit, and with a bit-error-rate (BER) of 10^-5.

The objective of this repository is to make accessible all the files related to the X-band transceiver, also called XCOMMS, which includes the hardware designs and all the necessary software of the subsystem as well as the different test boards that have been developed.

The subsystem is formed by:	SDR <-> Front-End <-> Antenna



# **Current state of the project**

At the moment, none of the PCB designs have been updated because they are pending to be translated from Altium to KiCad.
The project just includes the Front-End part.
The SDR and antenna parts are TBD. 

# **Project Tree Structure**

	XCOMMS
	├───Readme
	├───FrontEnd
	│   ├───Readme
	│   ├───HW
	│   │   ├───KiCad_Libraries
	│   │   ├───XCOMMS_Interface
	│   │   └───XCOMMS_RF
	│   └───SW
 	│	
	└───TesBoards
	    ├───PA_HMC952ALP5GE
	    │   ├───HW
	    │   └───SW
	    └───PLL_LMX2572
	    	├───Readme
		├───HW
		└───SW
		    ├───LMX2572
		    └───LMX2572_TICSPRO
		