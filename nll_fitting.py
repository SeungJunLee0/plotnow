import ROOT
from ROOT import RooFit, RooRealVar, RooGaussian, RooDataHist, RooArgList

def nll_fitting_with_reference(input_filenames, reference_filename, hist_name):
    # Define the observable
    x = RooRealVar("x", "Observable", -10, 10)

    # Define the Gaussian model parameters (shared across all fits)
    mean = RooRealVar("mean", "Mean", 1.0, 0.5, 2.0)
    sigma = RooRealVar("sigma", "Width", 1, 0.1, 10)

    # Define the Gaussian PDF
    gauss = RooGaussian("gauss", "Gaussian PDF", x, mean, sigma)

    # Create a canvas for plotting
    canvas = ROOT.TCanvas("canvas", "NLL Fits", 1200, 800)
    canvas.Divide(2, 2)  # Adjust the number of divisions based on the number of files (4 here)

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

    reference_hist = ref_hist.Clone()
    reference_hist.SetDirectory(0)
    ref_file.Close()

    # Process additional files
    for i, filename in enumerate(input_filenames):
        # Skip the reference file
        if filename == reference_filename:
            continue

        file = ROOT.TFile.Open(filename)
        if not file or file.IsZombie():
            print("Error: cannot open file %s" % filename)
            continue

        hist = file.Get(hist_name)
        if not hist:
            print("Error: cannot find histogram %s in file %s" % (hist_name, filename))
            file.Close()
            continue

        # Create a RooDataHist from the histograms
        data_hist = RooDataHist("data_hist", "Dataset with histogram", RooArgList(x), hist)
        ref_data_hist = RooDataHist("ref_data_hist", "Reference Dataset with histogram", RooArgList(x), reference_hist)

        # Perform the fit using the reference histogram as a model
        frame = x.frame(RooFit.Title("Fit for %s in file %s" % (hist_name, filename)))
        data_hist.plotOn(frame)
        ref_data_hist.plotOn(frame, RooFit.LineColor(ROOT.kRed), RooFit.MarkerColor(ROOT.kRed))

        gauss.fitTo(data_hist, RooFit.PrintLevel(-1))
        gauss.plotOn(frame)

        # Draw the frame
        canvas.cd(i + 1)
        frame.Draw()

        file.Close()

    # Update the canvas and save the plot
    canvas.Update()
    canvas.SaveAs("nll_fits_with_reference.png")
    print("NLL fits completed and saved as nll_fits_with_reference.png")

# Usage example
if __name__ == "__main__":
    reference_filename = "combined_output.root"
    input_filenames = [
        "combined_output.root",
        "combined_output_80.root",
        "combined_output_100.root",
        "combined_output_160.root",
        "combined_output_180.root"
    ]
    hist_name = "emu_InclusiveBTag_GreaterOneJet_mlb_minimax_yield"
    nll_fitting_with_reference(input_filenames, reference_filename, hist_name)

