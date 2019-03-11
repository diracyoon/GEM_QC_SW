# GEM_QC2_Long

## Authors
Inseok Yoon: inseok.yoon@cern.ch
Haneol Lee: haneol.lee@cern.ch for Painter::Draw


## Install CAEN CAENHVWrapper library
CAEN CAENHVWrapper library is necessary. To download it, please refer http://www.caen.it/csite/CaenProd.jsp?parent=43&idmod=835# Then you need to uncompress tar file, and install the library. To install, go to directory where the library is uncompressed. Then type
<pre>
sudo ./install.sh
</pre>

## Download and compile codes
CMake will take care of compling and installing.
<pre>
git clone git@github.com:diracyoon/GEM_QC_SW
cd GEM_QC_SW/Build
cmake CMakeLists.txt
make install
</pre>
Excutables shoud appear in Bin directory.

## Short explaination about the role of each class
### class HV_Controller
The role of class HV_Controller is communicating HV module.
### class Server
The class Server relays the IO requsts from Client to HV_Controller and vice versa. 
### class Client
The class Client communicates HV module via class Server as class QC_Base requests. 
### QC_Base
The class QC_Base does most of things for QC such as controlling class Client, making result log and ETC.    
### Preparation_QC_Long
Daughter class of QC_Base. Function for Preparation of QC Long are defined in here.
### QC_Long
Daughter class of QC_Base. QC Long specific functions are defined in here.
### Monitor
Class for monitoring and submitting log to elog.
### Watcher
Check QC_Long or Preparation_QC_Long processes are runnunig or not.
### Painter
Class for drawing.
### Submitter
Class for submitting log to elog.

## Usage
### Running Monitor
You need to set the environmental variables first.
<pre>
sudo -s
cd Macro
source Env.sh
./Run_Monitor.sh
</pre>	
### Running server
You shoud get su right first because CAEN wrapper library needs it. Then the environmental variables shuld be set. Now you are OK to run server. If the process is failed due to communication to HV module, just try to run one more time.
<pre>
sudo -s
cd Macro
source Env.sh
./Run_Server.sh
</pre>
### Running Preparation_QC_Long (QC_Long)
Prepare other terminal. You need su right again. The environmental variables are needed again. Then you are OK to run Preparation_QC_Long (QC_Long). Run_Preparation_QC_Long.sh (Run_QC_Long.sh) will ask foil id, HV channel number, relative humidity, temperaure and your name, one by one. Type those carefully.
<pre>
sudo -s
cd Macro
source Env.sh
./Run_Preparation_QC_Long.sh (./Run_QC_Long.sh)
</pre>
You can run the Preparation_QC_Long (QC_Long) as many as HV module channel. Every client with different channel number runs independently. If the server is not running or owned HV channel by other channel process, Run_Preparation_QC_Long.sh (Run_QC_Long.sh).sh script will notice it and kill itself. Anyway you should keep focus not to make any mistake.
## Output
Measured I-V curve will show up Output/Preparation (QC_Long) directory.
<pre>
File name of output file: (foil id)_(trial_number).result
structure of output file:
Overall information about the QC test.
##############################
Time and HV configuration for the QC test.
##############################
Time V_Set V_Mon I_Mon
##############################
number of trip during the QC test.
</pre>
### E-log
Log will be automatically submitted to e-log by class monitor when QC processes are done. Registration is needed to access the e-log. Don't be surprised when you access the e-log. Probably your web browser will warn you that the certificate of e-log is self signed. It's normal. You can ignore it. 
<pre>
URL: https://147.46.50.23:31417/QC
</pre>