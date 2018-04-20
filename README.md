# GEM_QC2_Long

## Authors
Inseok Yoon: inseok.yoon@cern.ch
Haneol Lee: haneol.lee@cern.ch

## Install CAEN CAENHVWrapper library
CAEN CAENHVWrapper library is necessary. To download it, please refer http://www.caen.it/csite/CaenProd.jsp?parent=43&idmod=835# Then you need to uncompress tar file, and install the library. To install, go to directory where the library is uncompressed. Then type
<pre>
sudo ./install.sh
</pre>

## Compile HV control codes
<pre>
git clone git@github.com:diracyoon/GEM_QC_SW
cd GEM_QC_SW/Code/Server
make
cd ../Client
make
</pre>
Excutables shoud appear.

## Short explaination about the role of each class
### class HV_Controller
The role of class HV_Controller is communicating HV module.
### class Server
The class Server relays the IO requsts from Client to HV_Controller and vice versa. 
### class Client
The class Client communicates HV module via class Server as class QC_Base requests. 
### QC_Base
The class QC_Base do most of things for QC such as controlling class Client, making result log and ETC.    
### QC_Long
Daughter class of QC_Base. QC Long specific functions are defined in here.

## Usage
### Running server
You shoud get su right first because CAEN wrapper library needs it. Then the environmental variables shuld be set. Now you are OK to run server. If the process is failed due to communication to HV module, just try to run one more time.
<pre>
sudo -s
cd Macro
source Env.sh
./Run_Server.sh
</pre>
### Running client
Prepare other terminal. You need su right again. The environmental variables are needed again. Then you are OK to run client. Run_Client will ask foil id, HV channel number, relative humidity, temperaure and your name. 
<pre>
sudo -s
cd Macro
source Env.sh
./Run_Client.sh
</pre>
You can run the client as many as HV module channel. Every client with different channel number runs independently.
If the server is not running or owned HV channel by other channel process, Run_Client.sh scrip will notice it and kill itself. Anyway you should keep focus not to make any mistake.
### Output
Measured I-V curve will show up Output/QC_Long directory.
<pre>
File name of output file: (runnumber)_(foil id).result
structure of output file:
Overall information about the QC test.
##############################
Time and HV configuration for the QC test.
##############################
Time V_Set V_Mon I_Mon
##############################
number of trip during the QC test.
</pre>
