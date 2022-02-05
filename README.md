# **Description**

The objective of this repository is to make accessible all the files related to the X-band transceiver, also called XCOMMS, which includes the PCB's designs and all the necessary code of the subsystem as well as the different test boards that have been developed.

The subsystem is formed by:	SDR <-> Front-End <-> Antenna



# **Current state of the project**

At the moment, none of the PCB designs have been updated because they are pending to be translated from Altium to KiCad.
The project just includes the Front-End part.
The SDR and antenna parts are TBD. 

# **Project structure**

The project tree structure is as follows:

	XCOMMS
	├───Readme
	├───FrontEnd
	│   ├───Readme
	│   ├───HW
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
		