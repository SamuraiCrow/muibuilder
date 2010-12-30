set dir Work:muibuilder
set mbdir $dir/mb
set catdir $dir/catalogs

# copies
cp builder $mbdir/MUIBuilder
cp catalogs/fran�ais/muibuilder_fran�ais.ct $mbdir/catalogs/Fran�ais.ct
cp catalogs/fran�ais/muibuilder.catalog $catdir/fran�ais
cp catalogs/fran�ais/MUIBuilder.cd $mbdir/catalogs
cp modules/GenCodeC $mbdir/modules
cp libs:muibuilder.library $dir/libs
cp env:MUIBuilder.env $dir/env

# actions
cd $mbdir/catalogs
flexcat MUIBuilder.cd NEWCTFILE MUIBuilder.ct
