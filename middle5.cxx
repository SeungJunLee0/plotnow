{


    TFile *file = new TFile("root_file/fitting.root","read"); 
    TString hist_name = "Minimum_value_Non";
	TH1F *hist1 =  (TH1F*) file -> Get(hist_name);
	hist1->SetTitle("");
	hist1->Fit("gaus");
	gStyle->SetOptFit(110);
	hist1->Draw();



	}


