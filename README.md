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

## Usage
#
You shoud get su right first because CAEN wrapper library needs it. Then the environmental variables shuld be set.
<pre>
sudo -s

</pre>
