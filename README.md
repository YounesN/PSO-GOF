# Optimization

There are two ways to run this script:
1. Let the script build your system by providing `pdb` file for single molecule, pack using `packmol`, provide `eq.conf` and `in.conf` so it will equilibriate and run the systems using these files. and etc. (more details below)
2. Pre-build your systems and include them in `PREBUILT` folder. Inside, should be more directories for each temperature in the format `<temperature>K` (e.g. `300K`).

# Pre-built systems:
___This is the default behavior. i.e. if `PREBUILT` does exist it will run using this method.___

Below is an example of directory `PREBUILT` which is required to run this type of system.
```
PREBUILT
├── RunFiles
|   ├── 300K
|   |   ├── in.conf
|   |   ├── SPCE_merged.psf
|   |   ├── water_mie.par
|   |   └── SPCE_BOX_0_restart.pdb
|   ├── 373K
|   |   ├── in.conf
|   |   ├── SPCE_merged.psf
|   |   ├── water_mie.par
|   |   └── SPCE_BOX_0_restart.pdb
|   ├── 450K
|   |   ├── in.conf
|   |   ├── SPCE_merged.psf
|   |   ├── water_mie.par
|   |   └── SPCE_BOX_0_restart.pdb
|   └── 550K
|       ├── in.conf
|       ├── SPCE_merged.psf
|       ├── water_mie.par
|       └── SPCE_BOX_0_restart.pdb
|
└── GOMC_CPU_NPT
```

Anything outside `RunFiles` directory will be copied into temperature directories before running simulations (e.g. in this case I put the executable outside so the script can update it everytime, in case there was a change to executable!). So, `GOMC_CPU_NPT` will be copied inside each temperature directory. _Also make sure you set the executable name inside `par.xml`_. This is to just to let the script know what executable to run. It will still copy the executable even if you don't specify it inside `par.xml`. However, if it will throw an error if it couldn't find the executable name.
```xml
<simulation>
  <executable>GOMC_CPU_NPT</executable>
</simulation>
```

For each temperature we need to set the temperatures and the expected density inside `par.xml` file. An example of input file for this method is included in `sample-prebuilt-par.xml`.
```xml
<data>
  <temperature>
    <temp>300</temp>
    <expt_dens>996.56</expt_dens>
  </temperature>
  <temperature>
    <temp>373</temp>
    <expt_dens>958.46</expt_dens>
  </temperature>
  <temperature>
    <temp>450</temp>
    <expt_dens>890.34</expt_dens>
  </temperature>
  <temperature>
    <temp>550</temp>
    <expt_dens>755.81</expt_dens>
  </temperature>
</data>
```
All of your `temperature` tags should be included inside `data`. Your actual temperature should be inside `temp` tag inside each `temperature` tag. And your experimental (expected) density inside `expt_dens` tag.

Finally, you need to set parameters and its range, what pattern to look for and where to find them.
Below is an example of required fields for parameters.
Consider the first parameter: It will look for `EEEEEEE` pattern inside `water_mie.par` file which should reside inside in temperature directories. (Next to `in.conf` files). Then it will replace it by a value between `30` and `120`. `kind` is set to `continuous` which means any value between `start` and `end` is valid. If it was set to `discrete` only integer numbers will be generated. This is useful to set value for `N`. Finally `name` is a field to recognize the parameter name which is useful to print simulation data.
```xml
<parameters>
  <parameter>
    <filename>water_mie.par</filename>
    <pattern>EEEEEEE</pattern>
    <name>epsilon</name>
    <kind>continuous</kind>
    <start>30</start>
    <end>120</end>
  </parameter>
  <parameter>
    <filename>water_mie.par</filename>
    <pattern>SSSSSSSSS</pattern>
    <name>sigma</name>
    <kind>continuous</kind>
    <start>3.0</start>
    <end>4.5</end>
  </parameter>
</parameters>
```

# Automated build:
- [ ] It works right now, however, this doc needs to be completed.