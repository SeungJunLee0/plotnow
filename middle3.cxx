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
#include "TFitResult.h"
#include "TMinuit.h"
#include <algorithm>



#include "RooRealVar.h"
#include "RooDataSet.h"
//#include "RooGaussian.h"
#include "RooPlot.h"
//#include "RooClassFactory.h"
#include "TROOT.h"
//
#include "TMinuit.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TLatex.h"


using namespace std;
//in the fitting error bar ==0.7
//Draw the minimum value of fitting's graph ( hist )
//


int main()
//int main(int argc, char **argv)
{
    system("./bear");


    TFile *f = new TFile("root_file/pesudo_axe.root","read"); 

	auto c1 = new TCanvas("c1","A Simple Graph with error bars",200,10,700,500);
	TFile *file = new TFile("root_file/fitting.root","recreate");
	TString hist_name[5] ={"Minimum_value","Lower_Limit","Upper_Limit","Mean-lower","Upper-mean"};
	TH1F* hist_random0 = new TH1F(hist_name[0],hist_name[0],100,0.5,2.0);
	hist_random0->GetXaxis()->SetTitle("GeV");
	hist_random0->GetYaxis()->SetTitle("Entries");
	TH1F* hist_random1 = new TH1F(hist_name[1],hist_name[1],100,0.5,2.0);
	TH1F* hist_random2 = new TH1F(hist_name[2],hist_name[2],100,0.5,2.0);
	TH1F* hist_random3 = new TH1F(hist_name[3],hist_name[3],100,0.0,0.5);
	TH1F* hist_random4 = new TH1F(hist_name[4],hist_name[4],100,0.0,0.5);
	for(int i=0; i <1000; i++){
		TString name_gr = "pesudo_experiment_" + TString::Itoa(i+1,10);
		TGraph *gr1 =  (TGraph*) f -> Get(name_gr);

		int num = gr1->GetN();
		double P_x[num],P_y[num];
		for (int k=0;k<num;k++){
			gr1->GetPoint(k,P_x[k],P_y[k]);
		}
		double ex[num]={0.0,}, ey[num]={0.0,};
		//auto *gr = new TGraphErrors();
		for (int k=0;k<num;k++){
		 	ey[k] =0.7;
			//gr->SetPoint(num,P_x[k],P_y[k]);
			//gr->SetPointError(num,ex[k],ey[k]);

			}
		//auto *gr = new TGraphErrors(num,P_x,P_y);
		auto *gr = new TGraphErrors(num,P_x,P_y,ex,ey);
		//gr->SetPoint(num,P_x,P_y);
		//gr->SetPointError(num,ex,ey);
		gr->SetTitle(name_gr);
		gr->SetName(name_gr);
    	gr->SetMarkerColor(4);
    	gr->SetMarkerStyle(8);
		gr->GetXaxis()->SetTitle("#Gamma_{top} (GeV)");
		gr->GetYaxis()->SetTitle("#Delta Negative-log-likelihood");
	    //gr->SetLineWidth(0);
		TFitResultPtr r = gr->Fit("pol2","S");  // TFitResultPtr contains the TFitResult
    	gr->Fit("pol2");
    	gr->Draw("P*");
		gr->Write();

    	//gr->Fit("pol4");

		//TFitResultPtr r = gr->Fit("pol4","S");  // TFitResultPtr contains the TFitResult

		double r0 = r->Parameter(0); 
		double r1 = r->Parameter(1); 
		double r2 = r->Parameter(2); 
		//double r3 = r->Parameter(3); 
		//double r4 = r->Parameter(4); 
		TF1 *f1 = new TF1("f1","pol2",0.66,2.00);
		f1->SetParameters(r0,r1,r2);
		////f1->SetParameters(r0,r1,r2,r3,r4);
		//f1->SetLineColor(kBlue);
		//f1->Draw("same");
		double mini = f1->GetMinimumX(0.5,2.0);
		hist_random0->Fill(mini);

		double mini_y = f1->Eval(mini);
		double mini1 = f1->GetX(mini_y+0.5, 0.7,  mini);
		double mini2 = f1->GetX(mini_y+0.5, mini, 2.00);
		cout << mini1 << "   "  << mini2 <<endl;
		hist_random1->Fill(mini1);
		hist_random2->Fill(mini2);
		hist_random3->Fill(mini-mini1);
		hist_random4->Fill(mini2-mini);

    }
///////////////////////////////////////about 160 /////////////////////////////////////////////////////////////
	TString hist_name_1[5] ={"Minimum_value_160","Lower_Limit_160","Upper_Limit_160","Mean-lower_160","Upper-mean_160"};
	TH1F* hist_random_1 = new TH1F(hist_name_1[0],hist_name_1[0],100,0.5,2.0);
	hist_random_1->GetXaxis()->SetTitle("GeV");
	hist_random_1->GetYaxis()->SetTitle("Entries");
	TH1F* hist_random_2 = new TH1F(hist_name_1[1],hist_name_1[1],100,0.5,2.0);
	TH1F* hist_random_3 = new TH1F(hist_name_1[2],hist_name_1[2],100,0.5,2.0);
	TH1F* hist_random_4 = new TH1F(hist_name_1[3],hist_name_1[3],100,0.0,0.5);
	TH1F* hist_random_5 = new TH1F(hist_name_1[4],hist_name_1[4],100,0.0,0.5);
	for(int i=0; i <1000; i++){
	//for(int i=1; i <2; i++){
		TString name_gr = "pesudo_experiment_160_" + TString::Itoa(i+1,10);
		TGraph *gr1 =  (TGraph*) f -> Get(name_gr);
		int num = gr1->GetN();
		double P_x[num],P_y[num];
		for (int k=0;k<num;k++){
			gr1->GetPoint(k,P_x[k],P_y[k]);
		}
		double ex[num]={0.0,}, ey[num]={0.0,};
		for (int k=0;k<num;k++){
		 	ey[k] =0.7;
			}
		auto *gr = new TGraphErrors(num,P_x,P_y,ex,ey);
		double find_good[11] ={0.0,};
		double find_test[11] ={0.0,};
		//double list_on[11] = {0.66,1.00,1.25,1.32,1.39,1.66,2.00};
		//for(int j=0;j<11;j++){
		//   	find_good[j]=gr->Eval(list_on[j]);
		//   	find_test[j]=gr->Eval(list_on[j]);
		//}
		//sort(find_test,find_test+11);
		//if(find_test[0] != find_good[2]) continue;
		gr->SetTitle(name_gr);
		gr->SetName(name_gr);
    	gr->SetMarkerColor(4);
    	gr->SetMarkerStyle(21);
	    //gr->SetLineWidth(0);
		gr->GetXaxis()->SetTitle("#Gamma_{top} (GeV)");
		gr->GetYaxis()->SetTitle("#Delta Negative-log-likelihood");
    	gr->Draw("P*");
		//double *ny = gr->GetY(); 
		//for(int j=0;j<5;j++) cout <<ny[j]<<endl;

    	gr->Fit("pol2");
		TFitResultPtr r = gr->Fit("pol2","S");  // TFitResultPtr contains the TFitResult

		double r0 = r->Parameter(0); 
		double r1 = r->Parameter(1); 
		double r2 = r->Parameter(2); 
		TF1 *f1 = new TF1("f1","pol2",0.66,2.00);
		f1->SetParameters(r0,r1,r2);
		f1->SetLineColor(kBlue);
		f1->Draw("same");
		gr->Write();
		double mini = f1->GetMinimumX(0.0,3.1);
		//double mini = f1->GetMinimumX(0.8,1.8);
		//cout << mini<<endl;
		hist_random_1->Fill(mini);

		double mini_y = f1->Eval(mini);
		double mini1 = f1->GetX(mini_y+0.5, 0.7,  mini);
		double mini2 = f1->GetX(mini_y+0.5, mini, 2.00);
		cout << mini1 << "   "  << mini2 <<endl;
		hist_random_2->Fill(mini1);
		hist_random_3->Fill(mini2);
		hist_random_4->Fill(mini-mini1);
		hist_random_5->Fill(mini2-mini);

    }
///////////////////////////////////////about 160 /////////////////////////////////////////////////////////////
///////////////////////////////////////about no  /////////////////////////////////////////////////////////////



	TString hist_name_n1[5] ={"Minimum_value_Non","Lower_Limit_no","Upper_Limit_no","Mean-lower_no","Upper-mean_no"};
	TH1F* hist_random_n1 = new TH1F(hist_name_n1[0],hist_name_n1[0],100,0.5,2.0);
	hist_random_n1->GetXaxis()->SetTitle("GeV");
	hist_random_n1->GetYaxis()->SetTitle("Entries");
	TH1F* hist_random_n2 = new TH1F(hist_name_n1[1],hist_name_n1[1],100,0.5,2.0);
	TH1F* hist_random_n3 = new TH1F(hist_name_n1[2],hist_name_n1[2],100,0.5,2.0);
	TH1F* hist_random_n4 = new TH1F(hist_name_n1[3],hist_name_n1[3],100,0.0,0.5);
	TH1F* hist_random_n5 = new TH1F(hist_name_n1[4],hist_name_n1[4],100,0.0,0.5);
	for(int i=0; i <1000; i++){
	//for(int i=1; i <2; i++){
		TString name_gr = "pesudo_experiment_no_" + TString::Itoa(i+1,10);
		TGraph *gr1 =  (TGraph*) f -> Get(name_gr);
		int num = gr1->GetN();
		double P_x[num],P_y[num];
		for (int k=0;k<num;k++){
			gr1->GetPoint(k,P_x[k],P_y[k]);
		}
		double ex[num]={0.0,}, ey[num]={0.0,};
		for (int k=0;k<num;k++){
		 	ey[k] =0.7;
			}
		auto *gr = new TGraphErrors(num,P_x,P_y,ex,ey);
		//double list_on[11] = {0.66,1.00,1.25,1.32,1.39,1.66,2.00};
		//for(int j=0;j<11;j++){
		//   	find_good[j]=gr->Eval(list_on[j]);
		//   	find_test[j]=gr->Eval(list_on[j]);
		//}
		//sort(find_test,find_test+11);
		//if(find_test[0] != find_good[2]) continue;
		gr->SetTitle(name_gr);
		gr->SetName(name_gr);
    	gr->SetMarkerColor(1);
    	gr->SetMarkerStyle(8);
    	gr->SetMarkerSize(0.8);
	    //gr->SetLineWidth(0);
		gr->GetXaxis()->SetTitle("#Gamma_{top} (GeV)");
		gr->GetYaxis()->SetTitle("#Delta Negative-log-likelihood");
    	gr->Draw("P");
		//double *ny = gr->GetY(); 
		//for(int j=0;j<5;j++) cout <<ny[j]<<endl;

    	gr->Fit("pol2");
		TFitResultPtr r = gr->Fit("pol2","S");  // TFitResultPtr contains the TFitResult

		double r0 = r->Parameter(0); 
		double r1 = r->Parameter(1); 
		double r2 = r->Parameter(2); 
		TF1 *f1 = new TF1("f1","pol2",0.66,2.00);
		f1->SetParameters(r0,r1,r2);
		f1->SetLineColor(kBlue);
		f1->Draw("same");
		gr->Write();
		double mini = f1->GetMinimumX(0.0,3.1);
		//double mini = f1->GetMinimumX(0.8,1.8);
		//cout << mini<<endl;
		hist_random_n1->Fill(mini);

		double mini_y = f1->Eval(mini);
		double mini1 = f1->GetX(mini_y+0.5, 0.7,  mini);
		double mini2 = f1->GetX(mini_y+0.5, mini, 2.00);
		cout << mini1 << "   "  << mini2 <<endl;
		hist_random_n2->Fill(mini1);
		hist_random_n3->Fill(mini2);
		hist_random_n4->Fill(mini-mini1);
		hist_random_n5->Fill(mini2-mini);

    }


///////////////////////////////////////about no  /////////////////////////////////////////////////////////////
	//c1->Draw();
	hist_random0->Write();
	hist_random1->Write();
	hist_random2->Write();
	hist_random3->Write();
	hist_random4->Write();



	hist_random_1->Write();
	hist_random_2->Write();
	hist_random_3->Write();
	hist_random_4->Write();
	hist_random_5->Write();


	hist_random_n1->Write();
	hist_random_n2->Write();
	hist_random_n3->Write();
	hist_random_n4->Write();
	hist_random_n5->Write();
	file->Close();


    c1->Modified(); c1->Update();

//    app.Run();

	return 0;
	}


