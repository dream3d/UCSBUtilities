# Find Elastic Modulus Mismatch  #


## Group (Subgroup) ##

Statistics Filters (Crystallographic)

## Description ##

This filter computes the modulus difference between the two grains bounding each surface mesh triangle.

## Parameters ##

None

## Required DataContainers ##

+ Volume
+ SurfaceMesh


## Required Objects ##

| Type | Default Name | Description | Comment |
|------|--------------|-------------|---------|
| Face | SurfaceMeshFaceLabels | N x 2 Col of signed integer |  |
| Feature (Volume) | Moduli | One (1) value (float) defining the elastic modulus of the **Feature** | | 


## Created Objects ##

| Type | Default Name | Description | Comment |
|------|--------------|-------------|---------|
| Face | SurfaceMeshDeltaModulus | float value | absolute value of difference in elastic modulus of bounding grains |

## Authors ##






## Example Pipelines ##



## License & Copyright ##

Please see the description file distributed with this **Plugin**
