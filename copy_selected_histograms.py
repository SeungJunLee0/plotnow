import ROOT


def combine_histograms(input_filenames, output_filename, hist_names):
    input_dir = "/cms/ldap_home/seungjun/TMW/CMSSW_10_6_32/src/TopAnalysis/Configuration/analysis/diLeptonic/miniTreeHistograms_2018/forPlotIt_bb4l/"
    combined_histograms = {}

    for filename in input_filenames:
        filename1 = input_dir+filename+".root"
        input_file = ROOT.TFile.Open(filename1, "READ")
        if not input_file or input_file.IsZombie():
            print("Error: cannot open file %s" % filename)
            continue



        for key in input_file.GetListOfKeys():
            obj = key.ReadObj()
            if isinstance(obj, ROOT.TH1F):
                hist_name = obj.GetName()
                if "emu" in hist_name and "mini" in hist_name:
                    #print(hist_name)
                    if "ttbartwbb4l" in filename1: 
                        obj.Scale(1.2)
                    if hist_name not in combined_histograms:
                        combined_histograms[hist_name] = obj.Clone()
                        combined_histograms[hist_name].SetDirectory(0)  # Detach from file
                    else:
                        combined_histograms[hist_name].Add(obj)

        input_file.Close()

    output_file = ROOT.TFile.Open(output_filename, "RECREATE")
    if not output_file or output_file.IsZombie():
        print("Error: cannot create file %s" % output_filename)
        return

    for hist_name, hist in combined_histograms.items():
        if hist:
            hist.Write()

    output_file.Close()
    print("Combined histograms saved to %s" % output_filename)



if __name__ == "__main__":
    #hist_names = ["hist1", "hist2", "hist3"]
    #copy_selected_histograms("input.root", "output.root", hist_names)
    input_dir = "/cms/ldap_home/seungjun/TMW/CMSSW_10_6_32/src/TopAnalysis/Configuration/analysis/diLeptonic/miniTreeHistograms_2018/forPlotIt_bb4l/"
    input_files_bb4l = ["ttbartwbb4linclusive","ttbartwbb4linclusive__WIDTH_100down","ttbartwbb4linclusive__WIDTH_180up","ttbartwbb4linclusive__WIDTH_160up","ttbartwbb4linclusive__WIDTH_80down"] ## 0~4
    #input_files_other = [input_files_bb4l[0]] ## 0~6
    input_file_num = int(input("Enter a number: "))
    input_files_other = ["ttbarbgFromLjets","ttbarbgFromHadronic","singletop","dy","ttbarX","diboson","wjets",input_files_bb4l[input_file_num]] ## 0~6
	
    #input_filenames = ["input1.root", "input2.root", "input3.root"]
    output_filename = "combined_output.root"
    if input_file_num == 1: output_filename = "combined_output_100.root"
    if input_file_num == 2: output_filename = "combined_output_180.root"
    if input_file_num == 3: output_filename = "combined_output_160.root"
    if input_file_num == 4: output_filename = "combined_output_80.root"
    
    hist_names = ["em"]

    combine_histograms(input_files_other, output_filename, hist_names)
