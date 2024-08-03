import ROOT

def print_histogram_names(input_filename, pattern):
    input_file = ROOT.TFile.Open(input_filename, "READ")
    if not input_file or input_file.IsZombie():
        print("Error: cannot open file")
        #print(f"Error: cannot open file {input_filename}")
        return

    for key in input_file.GetListOfKeys():
        obj = key.ReadObj()
        if isinstance(obj, ROOT.TH1):
            if pattern in obj.GetName() and "minimax" in obj.GetName():
                print(obj.GetName())

    input_file.Close()

if __name__ == "__main__":
    input_dir = "/cms/ldap_home/seungjun/TMW/CMSSW_10_6_32/src/TopAnalysis/Configuration/analysis/diLeptonic/miniTreeHistograms_2018/forPlotIt_bb4l/"
    input_filename = "ttbartwbb4linclusive.root"
    input_filename1 = input_dir+input_filename
    pattern = "emu" 
    print_histogram_names(input_filename1, pattern)

