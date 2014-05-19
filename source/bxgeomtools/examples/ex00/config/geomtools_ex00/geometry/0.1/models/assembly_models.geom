# -*- mode: conf-unix; -*-
# List of multi-properties (datatools::multi_properties):

#@description Some geometry models that implements an smart assembly of volumes

#@key_label   "name"
#@meta_label  "type"


#############################################################
[name="detector_stack.model" type="geomtools::stacked_model"]

#@config Configuration parameters to describe a detector unit as a stack of detection elements

############
# Geometry #
############ 

#@description The default length unit
length_unit : string = "mm"

#@description The stacking axis
stacked.axis    : string = "z"

#@description The number of stacked volumes
stacked.number_of_items : integer = 4

#@description The name of the geometry model at stacking slot 0
stacked.model_0 : string = "long_blue_cylinder0.model"

#@description The name of the geometry model at stacking slot 1
stacked.model_1 : string = "orange_medium_cylinder0.model"

#@description The name of the geometry model at stacking slot 2
stacked.model_2 : string = "green_box0.model"

#@description The name of the geometry model at stacking slot 3
stacked.model_3 : string = "red_medium_cylinder0.model"

#@description The name of the volume at stacking slot 0
stacked.label_0 : string = "Collector"

#@description The name of the volume at stacking slot 1
stacked.label_1 : string = "BackSensor"

#@description The name of the volume at stacking slot 2
stacked.label_2 : string = "Attenuator"

#@description The name of the volume at stacking slot 3
stacked.label_3 : string = "FrontSensor"

############
# Material #
############

#@description The name of the material that fills the box
material.ref    : string = "vacuum"

##############
# Visibility #
##############

#@description The recommended color for the display of the mother volume
visibility.color  : string  = "grey"

#@description The visibility hidden flag for the display of the mother volume
visibility.hidden : boolean = 0

###########
# Mapping #
###########

#@description The mapping directives for the "Attenuator" daughter volume
mapping.daughter_id.Attenuator : string = "[attenuator.gc]"

#@description The mapping directives for the "Collector" daughter volume
mapping.daughter_id.Collector  : string = "[collector.gc]"

#@description The mapping directives for the "BackSensor" daughter volume
mapping.daughter_id.BackSensor  : string = "[sensor.gc:position=0]"

#@description The mapping directives for the "FrontSensor" daughter volume
mapping.daughter_id.FrontSensor  : string = "[sensor.gc:position=1]"


###################################################################
[name="stack_with_insertion.model" type="geomtools::stacked_model"]

#@config Configuration parameters to describe a stack of volumes with special positioning

############
# Geometry #
############

#@description The default length unit
length_unit : string = "mm"

#@description The stacking axis
stacked.axis    : string = "z"

#@description The number of stacked volumes
stacked.number_of_items : integer = 4

#@description The name of the geometry model at stacking slot 0
stacked.model_0 : string = "blue_large_tube0.model"

#@description The position of the top contact point of the geometry model at stacking slot 0
stacked.limit_max_0 : real as length = 20.0 mm

#@description The name of the geometry model at stacking slot 1
stacked.model_1 : string = "orange_medium_cylinder0.model"

#@description The name of the geometry model at stacking slot 2
stacked.model_2 : string = "green_box0.model"

#@description The position of the bottom contact point of the geometry model at stacking slot 3
stacked.limit_min_3 : real as length = -130.0 mm

#@description The name of the geometry model at stacking slot 3
stacked.model_3 : string = "red_huge_tube0.model"

############
# Material #
############

#@description The name of the material that fills the box
material.ref    : string = "vacuum"

##############
# Visibility #
##############

#@description The recommended color for the display of the mother volume
visibility.color  : string  = "grey"

#@description The visibility hidden flag for the display of the mother volume
visibility.hidden : boolean = 0
