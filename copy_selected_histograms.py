import ROOT

def copy_selected_histograms(input_filename, output_filename, hist_names):
    # 원본 파일 열기
    input_dir = "/cms/ldap_home/seungjun/TMW/CMSSW_10_6_32/src/TopAnalysis/Configuration/analysis/diLeptonic/"
    input_file = ROOT.TFile.Open(input_filename, "READ")
    if not input_file or input_file.IsZombie():
        print(f"Error: cannot open file {input_filename}")
        return

    # 새 파일 만들기
    output_file = ROOT.TFile.Open(output_filename, "RECREATE")
    if not output_file or output_file.IsZombie():
        print(f"Error: cannot create file {output_filename}")
        input_file.Close()
        return

    # 원본 파일에서 모든 키를 순회
    for key in input_file.GetListOfKeys():
        obj = key.ReadObj()
        # 객체가 TH1 또는 그 하위 클래스인지 확인
        if isinstance(obj, ROOT.TH1):
            # 히스토그램 이름이 지정된 리스트에 있는지 확인
            if obj.GetName() in hist_names:
                # 새로운 파일로 이동하여 히스토그램 저장
                output_file.cd()
                obj.Write()

    # 파일 닫기
    input_file.Close()
    output_file.Close()

    print(f"Selected histograms copied to {output_filename}")


def combine_histograms(input_filenames, output_filename, hist_names):
    # 히스토그램 합산을 위한 사전 초기화
    input_dir = "/cms/ldap_home/seungjun/TMW/CMSSW_10_6_32/src/TopAnalysis/Configuration/analysis/diLeptonic/"
    combined_histograms = {name: None for name in hist_names}

    # 각 입력 파일에 대해 작업
    for filename in input_filenames:
        filename1 = input_dir+filename+".root"
        input_file = ROOT.TFile.Open(filename1, "READ")
        if not input_file or input_file.IsZombie():
            print(f"Error: cannot open file {filename}")
            continue

        for hist_name in hist_names:
            hist = input_file.Get(hist_name)
            if hist:
                # 합산 히스토그램 업데이트
                if combined_histograms[hist_name]:
                    if "ttbartw" in filename : hist.Scale(1.2)
                    combined_histograms[hist_name].Add(hist)
                else:
                    combined_histograms[hist_name] = hist.Clone()
                    combined_histograms[hist_name].SetDirectory(0)

        input_file.Close()

    # 새 파일에 합산된 히스토그램 저장
    output_file = ROOT.TFile.Open(output_filename, "RECREATE")
    if not output_file or output_file.IsZombie():
        print(f"Error: cannot create file {output_filename}")
        return

    for hist_name, hist in combined_histograms.items():
        if hist:
            hist.Write()

    output_file.Close()
    print(f"Combined histograms saved to {output_filename}")



# 사용 예시
if __name__ == "__main__":
    #hist_names = ["hist1", "hist2", "hist3"]
    #copy_selected_histograms("input.root", "output.root", hist_names)
    input_dir = "/cms/ldap_home/seungjun/TMW/CMSSW_10_6_32/src/TopAnalysis/Configuration/analysis/diLeptonic/"
	input_files_bb4l = ["ttbartwbb4linclusive","ttbartwbb4linclusive__WIDTH_100down","ttbartwbb4linclusive__WIDTH_180up","ttbartwbb4linclusive__WIDTH_160up","ttbartwbb4linclusive__WIDTH_80down"] ## 0~4
    input_files_other = ["ttbarbgFromLjets","ttbarbgFromHadronic","singletop","dy","ttbarX","diboson","wjets",input_files_bb4l[0]] ## 0~6
	
    #input_filenames = ["input1.root", "input2.root", "input3.root"]
    output_filename = "combined_output.root"
    hist_names = ["hist1", "hist2", "hist3"]

    combine_histograms(input_files_other, output_filename, hist_names)
