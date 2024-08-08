#!/bin/bash


pk="$(root-config --ldflags --cflags --libs --glibs) -lRooFitCore -lRooFit -lMinuit -o "

echo "1 : professor's powheg sampes "

echo " "

echo "2 : professor's madgraph samples"

echo ""

echo "3 :  my madgraph samples "


echo "Start First "
g++ middle1.cxx $pk l && ./l <<END
$1
END

echo "Start Second"
#g++ middle2.cxx $pk l && ./l <<END
g++ middle4.cxx $pk l && ./l <<END
$1
END


g++ middle3.cxx $pk l && ./l
#python Alldraw.py

echo " "
echo " "
bash cc.sh


echo " "
echo " "

echo "here is the number :  $1"
echo "1 : professor's powheg sampes "
echo " "
echo "2 : professor's madgraph samples"
echo ""
echo "3 :  my madgraph samples noMET "
echo ""
echo "4 :  my madgraph samples METcut "
rootbrowse root_file/fitting.root
rm -rf l
#eog /home/sjlee/test/pngfiles/pesudo_experiment_1.png
