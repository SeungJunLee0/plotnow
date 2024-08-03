import ROOT

def copy_selected_histograms(input_filename, output_filename, hist_names):
    # 원본 파일 열기
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

# 사용 예시
if __name__ == "__main__":
    hist_names = ["hist1", "hist2", "hist3"]
    copy_selected_histograms("input.root", "output.root", hist_names)

