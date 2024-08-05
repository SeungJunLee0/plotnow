import ROOT
import math




def calculate_nll_at_x(input_filenames, reference_filename, hist_name, x_values):
    # List to store NLL results
    nll_results = {}

    # Open the reference file and retrieve the histogram
    ref_file = ROOT.TFile.Open(reference_filename)
    if not ref_file or ref_file.IsZombie():
        print("Error: cannot open reference file %s" % reference_filename)
        return

    ref_hist = ref_file.Get(hist_name)
    if not ref_hist:
        print("Error: cannot find histogram %s in reference file %s" % (hist_name, reference_filename))
        ref_file.Close()
        return

    # Clone the reference histogram and set directory to 0
    reference_hist = ref_hist.Clone()
    reference_hist.SetDirectory(0)
    ref_file.Close()

    # Calculate NLL for each file at the specified x values
    for filename, x_val in x_values.items():
        file = ROOT.TFile.Open(filename)
        if not file or file.IsZombie():
            print("Error: cannot open file %s" % filename)
            continue

        hist = file.Get(hist_name)
        if not hist:
            print("Error: cannot find histogram %s in file %s" % (hist_name, filename))
            file.Close()
            continue

        # Find the bin corresponding to the x value in the reference histogram
        bin_idx = reference_hist.GetXaxis().FindBin(x_val)
        print(reference_hist.GetNbinsX())
        ref_bin_content = reference_hist.GetBinContent(bin_idx)
        bin_content = hist.GetBinContent(bin_idx)

        # Calculate NLL for this bin
        if bin_content > 0 and ref_bin_content > 0:
            nll = ref_bin_content * math.log(ref_bin_content / bin_content) - ref_bin_content + bin_content
        else:
            nll = float('inf')  # Inf for undefined values

        nll_results[filename] = (x_val, nll)
        file.Close()

    # Plotting
    canvas = ROOT.TCanvas("canvas", "NLL Plot", 800, 600)
    legend = ROOT.TLegend(0.1, 0.7, 0.3, 0.9)

    graph = ROOT.TGraph(len(nll_results))
    colors = [ROOT.kRed, ROOT.kBlue, ROOT.kGreen, ROOT.kMagenta]

    for i, (filename, (x_val, nll)) in enumerate(nll_results.items()):
        graph.SetPoint(i, x_val, nll)
        graph.SetMarkerStyle(20)
        graph.SetMarkerColor(colors[i % len(colors)])
        legend.AddEntry(graph, filename, "p")

    graph.Draw("AP")
    graph.GetXaxis().SetTitle("x value")
    graph.GetYaxis().SetTitle("Negative Log Likelihood (NLL)")
    legend.Draw()

    canvas.Update()
    canvas.SaveAs("nll_vs_x_point_plot.png")
    print("NLL plot saved as nll_vs_x_point_plot.png")

# Usage example
if __name__ == "__main__":
    reference_filename = "combined_output.root"
    input_filenames = [
        "combined_output_80.root",
        "combined_output_100.root",
        "combined_output_160.root",
        "combined_output_180.root"
    ]
    hist_name = "emu_TwoBTag_TwoJet_mlb_minimax_yield"
    #hist_name = "emu_InclusiveBTag_GreaterOneJet_mlb_minimax_yield"
    x_values = {
        "combined_output_80.root": 0.8,
        "combined_output_100.root": 1.0,
        "combined_output_160.root": 1.6,
        "combined_output_180.root": 1.8
    }
    calculate_nll_at_x(input_filenames, reference_filename, hist_name, x_values)

