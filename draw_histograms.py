import ROOT

def draw_histograms(file_names, hist_name):
    # Create a canvas
    canvas = ROOT.TCanvas("canvas", "Histograms Comparison", 800, 600)

    # Define colors for each histogram
    colors = [ROOT.kRed, ROOT.kBlue, ROOT.kGreen, ROOT.kMagenta, ROOT.kCyan]

    # List to store histograms
    histograms = []

    # Loop over file names and retrieve histograms
    for i, file_name in enumerate(file_names):
        file = ROOT.TFile.Open(file_name)
        if not file or file.IsZombie():
            print("Error: cannot open file {}".format(file_name))
            continue

        hist = file.Get(hist_name)
        if not hist:
            print("Error: cannot find histogram {} in file {}".format(hist_name, file_name))
            file.Close()
            continue

        # Clone the histogram and set attributes
        hist = hist.Clone()
        hist.SetDirectory(0)  # Detach from the file
        hist.SetLineColor(colors[i % len(colors)])
        hist.SetLineWidth(2)
        histograms.append(hist)

        file.Close()

    # Draw histograms on the same canvas
    if histograms:
        histograms[0].Draw("hist")  # Draw the first histogram
        for hist in histograms[1:]:
            hist.Draw("hist same")  # Draw the rest on the same canvas

        # Add a legend
        legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
        for i, hist in enumerate(histograms):
            legend.AddEntry(hist, file_names[i], "l")
        legend.Draw()

        # Update the canvas and save the plot
        canvas.Update()
        canvas.SaveAs("histograms_comparison.png")
        print("Histograms comparison saved as histograms_comparison.png")
    else:
        print("No histograms were drawn.")

# Usage example
if __name__ == "__main__":
    file_names = [
        "combined_output_100.root",
        "combined_output_160.root",
        "combined_output_180.root",
        "combined_output_80.root",
        "combined_output.root"
    ]
    hist_name = "emu_TwoBTag_GreaterOneJet_mlb_minimax_yield"  # Replace with the actual histogram name
    draw_histograms(file_names, hist_name)

