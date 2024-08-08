#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TRint.h"
#include "TCanvas.h"
#include "stdio.h"
#include "TLegend.h"
#include "TString.h"
#include "TRandom.h"
#include "stdlib.h"
#include "Fit/BinData.h"
#include "TMinuit.h"
#include <algorithm>


#include "RooRealVar.h"
#include "RooDataSet.h"
//#include "RooGaussian.h"
#include "RooPlot.h"
//#include "RooClassFactory.h"
//#include "TROOT.h"
//
#include "TMinuit.h"

using namespace std;
using namespace RooFit;




//double fcn(double* k, double* lamda, double* par){
//	double value = k*log(par*lamda) + par*lamda ;
//	return value;
//    }
int lamda[50] = {0,};
//int lamda[500] = {0,};
int k[50] = {0,};
//int k[500] = {0,};


void fcn1(int &npar, double *gin,double &f, double *par, int iflag)
{
/* chi^2 */
  double chi2 = 0;
  double xx;
  for (int i=0;i<50;i++) {
    if (k[i]>0) {
      xx  = par[0]*lamda[i];
      chi2 += pow(k[i]-xx,2)/k[i];
    }
  }
  f = chi2;
}





void fcn(int &npar, double *gin,double &f, double *par, int iflag){
	double nega =0.0;
	for(int i =0;i<15;i++){
		if(lamda[i] !=0 ) nega += -(double)k[i]*log(par[0]*(double)lamda[i]) + par[0]*(double)lamda[i];
		//for(int j =i*10;j< i*10+10;j++){
		//	if(lamda[j] !=0) nega += -(double)k[j]*log(par[0]*(double)lamda[j]) + par[0]*(double)lamda[j];
		//}
		//if(lamda[i]!=0) cout << "labda : " << lamda[i] << "   k  : "<< k[i]<<endl;
	}
	//cout << "this  is par : "<<par[0]<< endl;
	f = nega;
}


double *cal(TString width, int rebin){
   
	TString width_name = "combined_output_"+width+".root";
	TFile *f = new TFile(width_name,"read");

	//TString width = "1_32";
	TString width_name1 = "combined_output_130.root";//"/home/sjlee/DATA/hists_powheg_pythia8_CP5_delphes_" + width + "mumu.root";
	TFile *f1 = new TFile(width_name1,"read");
	TString data_name = "emu_TwoBTag_TwoJet_mlb_minimax_yield";
	TH1F *hist_data =  (TH1F*) f1 -> Get( data_name);
	hist_data->Rebin(rebin);

    static double value_nega[1000] ={0,};
	for(int i=0; i <1; i++){
	TString hist_name = "emu_TwoBTag_TwoJet_mlb_minimax_yield"; 
	TH1F *hist_mc_1 =  (TH1F*) f -> Get(hist_name);
	hist_mc_1->Rebin(rebin);
	//for(int e=0;e<100;e++){
	for(int e=0;e<15;e++){
		k[e] = (hist_mc_1->GetBinContent(e+1));
		lamda[e] = (hist_data->GetBinContent(e+1));
	}

	TMinuit *gMinuit = new TMinuit(5);  //initialize TMinuit with a maximum of 5 params
	gMinuit->SetPrintLevel(-1);
	gMinuit->SetFCN(fcn);
    double arglist[10];
	int ierflg = 0;

	//arglist[0] = 1;
	//gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

	//  Set starting values and step sizes for parameters
	//static double vstart[1] = {1.};
	static double vstart[1] = {0.10};
	//static double vstart[1] = {0.106};
	static double step[1] = {0.0001};
	static double bmin[1] = {0.0001};// if zero nan problem
	static double bmax[1] = {2};
	gMinuit->mnparm(0, "a1", vstart[0], step[0],bmin[0] ,bmax[0],ierflg);
	gMinuit->SetErrorDef(0.5);//set error definition 1=chi 0.5 negativeloglikelihood 
	//arglist[0] = 1;
	arglist[0] = 50000;// for count!!
	//arglist[1] = 10; // function changes smaller than this value than find the minimize that 
	arglist[1] = 0.0001; // function changes smaller than this value than find the minimize that 
	//Minimization strategy
	gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);
	//cout << " maxamx :   "<<gMinuit->GetMaxIterations()<< endl;

	//  Print results
	double amin,edm,errdef;
	int nvpar,nparx,icstat;
	gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
    //printf("%f\n",amin);
    //printf("%lf\n",amin);
    value_nega[i] = amin;
	//cout << "amin : "<< amin<< endl;
	//printf("amin : %lf\n",amin);
	TString ds;
	double value,sstep,bbmin,bbmax;
	gMinuit->mnpout(0, ds, value, sstep,bbmin , bbmax,ierflg);
	//cout << "value : "<<value<<endl;
    //cout << "bmin :  "<< bbmin<<endl;	
	}
    f->Close();
	f1->Close();
	return value_nega;
}

double *cal_16(TString width,int rebin){
   
	TString width_name = "root_file/poisson_viking.root";
	TFile *f = new TFile(width_name,"read");

	//TString width = "1_32";
	TString width_name1 = "root_file/template_file.root";//"/home/sjlee/DATA/hists_powheg_pythia8_CP5_delphes_" + width + "mumu.root";
	TFile *f1 = new TFile(width_name1,"read");
	TString data_name = "hblmass0_" + width;
	TH1F *hist_data =  (TH1F*) f1 -> Get( data_name);
	hist_data->Rebin(rebin);

    static double value_nega[1000] ={0,};
	for(int i=0; i <1000; i++){
	TString hist_name = "hist_"+ TString::Itoa(i,10); 
	TH1F *hist_mc_1 =  (TH1F*) f -> Get(hist_name);
	hist_mc_1->Rebin(rebin);
	//for(int e=0;e<20;e++){
	for(int e=0;e<16;e++){
		k[e] = (hist_mc_1->GetBinContent(e+1));
		lamda[e] = (hist_data->GetBinContent(e+1));
	}

	TMinuit *gMinuit = new TMinuit(1);  //initialize TMinuit with a maximum of 5 params
	gMinuit->SetPrintLevel(-1);
	gMinuit->SetFCN(fcn);
    double arglist[10];
	int ierflg = 0;

	//arglist[0] = 1;
	//gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

	//  Set starting values and step sizes for parameters
	static double vstart[1] = {0.106};
	static double step[1] = {0.0001};
	static double bmin[1] = {0.0001};// if zero nan problem
	static double bmax[1] = {2};
	gMinuit->mnparm(0, "a1", vstart[0], step[0],bmin[0] ,bmax[0],ierflg);
	gMinuit->SetErrorDef(0.5);//set error definition 1=chi 0.5 negativeloglikelihood 
	//arglist[0] = 1;
	arglist[0] = 50000;// for count!!
	//arglist[1] = 10; // function changes smaller than this value than find the minimize that 
	arglist[1] = 0.0001; // function changes smaller than this value than find the minimize that 
	//Minimization strategy
	gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);
	//cout << " maxamx :   "<<gMinuit->GetMaxIterations()<< endl;

	//  Print results
	double amin,edm,errdef;
	int nvpar,nparx,icstat;
	gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
    //printf("%f\n",amin);
    //printf("%lf\n",amin);
    value_nega[i] = amin;
	//cout << "amin : "<< amin<< endl;
	//printf("amin : %lf\n",amin);
	TString ds;
	double value,sstep,bbmin,bbmax;
	gMinuit->mnpout(0, ds, value, sstep,bbmin , bbmax,ierflg);
	}
    f->Close();
	f1->Close();
	return value_nega;
}

double *cal_no(TString width,int rebin){
   
	TString width_name = "root_file/poisson_viking.root";
	TFile *f = new TFile(width_name,"read");

	//TString width = "1_32";
	TString width_name1 = "root_file/template_file.root";//"/home/sjlee/DATA/hists_powheg_pythia8_CP5_delphes_" + width + "mumu.root";
	TFile *f1 = new TFile(width_name1,"read");
	TString data_name = "hblmass0_" + width;
	TH1F *hist_data =  (TH1F*) f1 -> Get( data_name);
	hist_data->Rebin(rebin);

    static double value_nega[1000] ={0,};
	for(int i=0; i <1000; i++){
	TString hist_name = "hist_"+ TString::Itoa(i,10); 
	TH1F *hist_mc_1 =  (TH1F*) f -> Get(hist_name);
	hist_mc_1->Rebin(rebin);
	//for(int e=0;e<20;e++){
	for(int e=14;e<50;e++){
		k[e] = (hist_mc_1->GetBinContent(e+1));
		lamda[e] = (hist_data->GetBinContent(e+1));
	}

	TMinuit *gMinuit = new TMinuit(1);  //initialize TMinuit with a maximum of 5 params
	gMinuit->SetPrintLevel(-1);
	gMinuit->SetFCN(fcn);
    double arglist[10];
	int ierflg = 0;

	//arglist[0] = 1;
	//gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

	//  Set starting values and step sizes for parameters
	static double vstart[1] = {0.106};
	static double step[1] = {0.0001};
	static double bmin[1] = {0.0001};// if zero nan problem
	static double bmax[1] = {2};
	gMinuit->mnparm(0, "a1", vstart[0], step[0],bmin[0] ,bmax[0],ierflg);
	gMinuit->SetErrorDef(0.5);//set error definition 1=chi 0.5 negativeloglikelihood 
	//arglist[0] = 1;
	arglist[0] = 50000;// for count!!
	//arglist[1] = 10; // function changes smaller than this value than find the minimize that 
	arglist[1] = 0.0001; // function changes smaller than this value than find the minimize that 
	//Minimization strategy
	gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);
	//cout << " maxamx :   "<<gMinuit->GetMaxIterations()<< endl;

	//  Print results
	double amin,edm,errdef;
	int nvpar,nparx,icstat;
	gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
    //printf("%f\n",amin);
    //printf("%lf\n",amin);
    value_nega[i] = amin;
	//cout << "amin : "<< amin<< endl;
	//printf("amin : %lf\n",amin);
	TString ds;
	double value,sstep,bbmin,bbmax;
	gMinuit->mnpout(0, ds, value, sstep,bbmin , bbmax,ierflg);
	}
    f->Close();
	f1->Close();
	return value_nega;
}


int main()
{
    int a =0;
    TFile *file = new TFile("root_file/pesudo_axe.root","recreate"); 

	TString width[5] ={"80",//0
					   "100",//2
					   "130",//5
					   "160",//9
					   "180"//11
	};




	int n_points = 5;
	double x_vals[5] = {0.80,
						 1.00,
						 1.30,
						 1.60,
						 1.80
	};


	double val[5][1]={0.0,};
	double mini[1]={0.0,};
	for(int j =0;j<5;j++){
		double *instant_val=cal(width[j],2);
		for(int k =0;k<1;k++) {
			val[j][k] =instant_val[k];
			if(mini[k] > val[j][k]) mini[k] = val[j][k];
		}
	}
	for(int i =0;i<1;i++){
	    double y_vals[5] = {val[0 ][i]-mini[i],
			                 val[1 ][i]-mini[i],
							 val[2 ][i]-mini[i],
							 val[3 ][i]-mini[i],
							 val[4 ][i]-mini[i]
		};	
		TGraph *gr = new TGraph(n_points,x_vals,y_vals);
		TString name_gr = "pesudo_experiment_" + TString::Itoa(i+1,10); 
		gr->SetTitle(name_gr);
		gr->SetName(name_gr);
		gr->SetLineWidth(0);
		gr->Draw("ap*");
		//gr->Draw("*");
		file->cd();
		gr->Write();

	}
	for(int i =0;i<5;i++) cout << val[i][1]-mini[1]<<endl;

	//val[11][1]={0.0,};
	//mini[1]={0.0,};
	//for(int j =0;j<5;j++){
	//	double *instant_val=cal_16(width[j],1);
	//	for(int k =0;k<1;k++) {
	//		val[j][k] =instant_val[k];
	//		if(mini[k] > val[j][k]) mini[k] = val[j][k];
	//	}
	//}
	//for(int i =0;i<1;i++){
	//    double y_vals[5] = {val[0 ][i]-mini[i],
	//		                 val[1 ][i]-mini[i],
	//						 val[2 ][i]-mini[i],
	//						 val[3 ][i]-mini[i],
	//						 val[4 ][i]-mini[i]
	//	};	
	//	TGraph *gr = new TGraph(n_points,x_vals,y_vals);
	//	TString name_gr = "pesudo_experiment_160_" + TString::Itoa(i+1,10); 
	//	gr->SetTitle(name_gr);
	//	gr->SetName(name_gr);
	//	gr->SetLineWidth(0);
	//	gr->Draw("*");
	//	file->cd();
	//	gr->Write();

	//}


	//double val1[11][1000]={0.0,};
	//double mini1[1000]={0.0,};
	//for(int j =0;j<11;j++){
	//	double *instant_val1=cal_no(width[j],rebin);
	//	for(int k =0;k<1000;k++) {
	//		val1[j][k] =instant_val1[k];
	//		if(mini1[k] > val1[j][k]) mini1[k] = val1[j][k];
	//	}
	//}
	//for(int i =0;i<1000;i++){
	//    double y_vals[11] = {val1[0 ][i]-mini1[i],
	//		                 val1[1 ][i]-mini1[i],
	//						 val1[2 ][i]-mini1[i],
	//						 val1[3 ][i]-mini1[i],
	//						 val1[4 ][i]-mini1[i],
	//						 val1[5 ][i]-mini1[i],
	//					//	 val1[6 ][i]-mini1[i], 
	//						 val1[6 ][i]-mini1[i], 
	//						 val1[7 ][i]-mini1[i], 
	//						 val1[8 ][i]-mini1[i],
	//						 val1[9 ][i]-mini1[i],
	//						 val1[10][i]-mini1[i]
	//	};	
	//	TGraph *gr = new TGraph(n_points,x_vals,y_vals);
	//	TString name_gr = "pesudo_experiment_no_" + TString::Itoa(i+1,10); 
	//	gr->SetTitle(name_gr);
	//	gr->SetName(name_gr);
	//	gr->SetLineWidth(0);
	//	gr->Draw("*");
	//	file->cd();
	//	gr->Write();

	//}
	//
	//for(int i =0;i<11;i++) cout << val[i][1]-mini[1]<<endl;


	file->Close();


	return 0;
	}


