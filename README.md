Analysis framework
==================

This project combines data manipulation modules

1. AppFramework --- the core unit controlling code execution flow (initialization, scheduling, and messaging between the processing modules)

2. AnModBasic --- specific input and output modules
<br>
InputModule --- module that reads _Events_ and _ParameterSets_ branches from a ROOT tree of a typical CMS EDM file
<br>
OutputModule --- module that write selected variables (integers and doubles) into CSV files and flat ROOT trees

3. Logistics --- the database of samples, reweighting schemes, scaling factors, etc.
<br>
SampleHelper --- dictionary (AppAgent, not an AppModule) that associate of sample name with physical location of files for InputModule

4. AnObjects --- analysis objects comprising an event (leptons, jets, triggers...)

5. Readers --- collection of modules reading the EDM objects 

6. workdir --- working directory with a short tutorial
