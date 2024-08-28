#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TMinuit.h"
#include "TGraph.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int count1 = 0;
TH1F* hist_data = nullptr;
TH1F* hist_mc_1 = nullptr;

void fcn(int &npar, double *gin, double &f, double *par, int iflag) {
    double nega = 0.0;
    for (int i = 1; i <= hist_data->GetNbinsX(); ++i) {
        double dataContent = hist_data->GetBinContent(i);
        double mcContent = hist_mc_1->GetBinContent(i);
        if (dataContent > 0) {
            nega += -mcContent * log(par[0] * dataContent) + par[0] * dataContent;
        }
    }
    f = nega;
}

double cal(TString width, int count1) {
    static double value_nega = 0.0;

    TMinuit *gMinuit = new TMinuit(5);  // Initialize TMinuit with 1 parameter
    gMinuit->SetFCN(fcn);
    gMinuit->SetPrintLevel(-1);

    double arglist[10];
    int ierflg = 0;

    static double vstart = 0.10;
    static double step = 0.0001;
    static double bmin = 0.0001;
    static double bmax = 10.0;
    gMinuit->mnparm(0, "a1", vstart, step, bmin, bmax, ierflg);
    gMinuit->SetErrorDef(0.5);

    arglist[0] = 50000;
    arglist[1] = 0.0001;
    gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);

    double amin, edm, errdef;
    int nvpar, nparx, icstat;
    gMinuit->mnstat(amin, edm, errdef, nvpar, nparx, icstat);

    value_nega = amin;

    delete gMinuit;

    return value_nega;
}

int main() {

    TFile *file = new TFile("root_file/pesudo_axe.root", "recreate");

    TString widths[5] = { "80", "100", "130", "160", "180" };

    int n_points = 5;
    double x_vals[5] = { 0.80, 1.00, 1.30, 1.60, 1.80 };

    double val[5][1000] = {0.0};
    double mini[1000] = {0.0};

    TFile *f = new TFile("root_file/poisson_viking.root", "read");
    TFile *f1 = new TFile("root_file/template_file.root", "read");

    for (int k = 0; k < 1000; k++) {
        for (int j = 0; j < 5; j++) {
            count1 = k;
            TString data_name = "hblmass0_" + widths[j];
            hist_data = (TH1F*)f1->Get(data_name);

            TString hist_name = "hist_" + TString::Itoa(count1, 10);
            hist_mc_1 = (TH1F*)f->Get(hist_name);

            val[j][k] = cal(widths[j], k);
            mini[k] = 0.0;
        }
    }

    for (int i = 0; i < 1000; i++) {
        double y_vals[5] = {
            val[0][i] - mini[i],
            val[1][i] - mini[i],
            val[2][i] - mini[i],
            val[3][i] - mini[i],
            val[4][i] - mini[i]
        };

        TGraph *gr = new TGraph(n_points, x_vals, y_vals);
        TString name_gr = "pesudo_experiment_" + TString::Itoa(i+1, 10);
        gr->SetTitle(name_gr);
        gr->SetName(name_gr);
        gr->SetLineWidth(0);
        gr->Draw("ap*");

        file->cd();
        gr->Write();

        delete gr;  // Clean up memory
    }

    f->Close();
    f1->Close();
    file->Close();

    delete f;
    delete f1;
    delete file;

    return 0;
}

