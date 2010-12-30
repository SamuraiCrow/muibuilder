set dir Work:muibuilder
set mbdir $dir/mb
set catdir $dir/catalogs

# copies
cp builder $mbdir/MUIBuilder
cp catalogs/français/muibuilder_français.ct $mbdir/catalogs/Français.ct
cp catalogs/français/muibuilder.catalog $catdir/français
cp catalogs/français/MUIBuilder.cd $mbdir/catalogs
cp modules/GenCodeC $mbdir/modules
cp libs:muibuilder.library $dir/libs
cp env:MUIBuilder.env $dir/env

# actions
cd $mbdir/catalogs
flexcat MUIBuilder.cd NEWCTFILE MUIBuilder.ct
