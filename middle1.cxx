#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TRint.h"
#include "TCanvas.h"
#include "stdio.h"
#include "TLegend.h"
#include "TString.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "stdlib.h"
#include "Fit/BinData.h"
using namespace std;
using namespace ROOT::Fit;


int make_hist(TString width, int a){

	
	TString dir	= "/home/sjlee/test/";


    TString hist_name;
   

	if(a <=2) hist_name = "hblmass0_emu";
	else hist_name ="Invariant_Mass_em";

	if(a == 1) dir = dir+"powheg/";
	if(a == 2) dir = dir+"madgraph/";
	if(a == 3) dir = dir+"madgraph_me_noMET/";
	if(a == 4) dir = dir+"madgraph_me_METcut/";


    TString width_name = dir+width+"emu.root";//powheg met cut in emu channel
	
	TFile *f080 = new TFile(width_name,"read");
    TH1F *hist_080 =  (TH1F*) f080 -> Get(hist_name);
	hist_080->SetDirectory(0);

	width.TString::Remove(1,1);
	TString hist_name2 = "hblmass0_"+width;

    hist_080->SetName(hist_name2);
	double count = hist_080->GetEntries();
	hist_080->Scale(1400000.0/count);

    TFile *file =new TFile("root_file/template_file.root","update");
    hist_080->Write(); 
	file->Close();
    f080->Close();
	return 0;

}



//int main(int argc, char **argv)
int main()
{
    system("./bear");
    //TRint app("app", &argc, argv);
	//
	//
	int a=0;
	cin >> a;


	int count = 0;
    TString width[12] = {"0_80",//  0
						 "0_90",//  1
						 "1_00",//  2
						 "1_10",//  3
						 "1_20",//  4
						 "1_30",
						 "1_32",//  5
						 "1_40",//  6 
						 "1_50",//  7
						 "1_60",//  8
						 "1_70", //  9
						 "1_80" //  10
	};
    TString decay_type[5] = {"emu","mumu","mutau","taumu","tautau"};
    //TString hist_name = "Mass_l_jb_gen";
    
    TCanvas* c = new TCanvas("c", "Something", 0, 0, 800, 600);


    TFile *file =new TFile("root_file/template_file.root","recreate");
	file->Close();
    
	for(int i =0;i<12;i++){
		TString width_n= width[i];
		make_hist(width_n,a);
	}
	


    TFile *file_basic =new TFile("root_file/template_file.root","read");
	TH1F *basic = (TH1F *) file_basic->Get("hblmass0_132"); 
    TFile *file_poisson =new TFile("root_file/poisson_viking.root","recreate");
    int bin = 72000;
    //int bin = 90000;//42597;
    //int bin = (27480+918);
	int bins=0;
	if (a<=2) bins =500;
	else bins =100;

	TRandom3 *rd = new TRandom3;
    rd->SetSeed(1);

    TString hist_name = "Invariant_Mass";

    for(int i=0; i <1000; i++){
    //for(int i=0; i <100; i++){ // origin
    	int ran1 = rd->Poisson(bin);
		TH1F *exp = (TH1F*)basic ->Clone();
    	//int ran2 = gRandom->Poisson(bin_DY);
    	//int ran3 = gRandom->Poisson(bin_DY_v2);
     	hist_name = "hist_"+ TString::Itoa(i,10); 
     	TString hist_name2 = "Template Method "+ TString::Itoa(i,10)+" : 1.32  GeV"; 

     	TH1F* hist_random = new TH1F(hist_name,hist_name2,bins,0,500);
     	//TH1F* hist_random = new TH1F(hist_name,hist_name2,100,0,500);
		//for(int k =0;k<ran1;k++) hist_random->Fill(basic->GetRandom());
		for(int k =0 ; k<ran1;k++){
			double mlb = exp->GetRandom();
			hist_random->Fill(mlb);
		}
        //hist_random->FillRandom(basic, ran1);
    	//hist_random->FillRandom(DYY, ran2);
    	//hist_random->FillRandom(hist_dy_v2, ran3);
		file_poisson->cd();
     	hist_random->Write();
    }

    file_poisson->Close(); 
    //c->Modified(); c->Update();

    //app.Run();

    return 0;
}


