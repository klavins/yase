#include "fft.hh"
#include "yase.hh"

namespace yase {


    FFT::FFT(int size) : size(size), n(0) {

        signal = add_input("signal");
        ready = add_output("ready");
        in = (double*) fftw_malloc(sizeof(double) * size);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (size/2+1));
        plan = fftw_plan_dft_r2c_1d(size, in, out, FFTW_MEASURE);        

    }

    FFT::~FFT() {
        fftw_destroy_plan(plan);
        fftw_free(in); 
        fftw_free(out);        
    }

    void FFT::init() {}

    void FFT::update() {

        if ( n < size ) {
            in[n++] = inputs[signal];            
        } 
        
        if ( n == size ) {
            fftw_execute(plan);
            n = 0;
            outputs[ready] = 1.0;
        } else {
            outputs[ready] = 0.0;
        }

    }

    fftw_complex * FFT::get() {
      return out;
    }

    void FFT::ascii() {
        for ( int i=0; i<size/2+1; i++ ) {
            double a = out[i][0], b = out[i][1], m = sqrt(a*a+b*b) / size;
            std::cout << (int) (SAMPLE_RATE * 1.0 * i / size) << "\t";
            std::cout << 2 * m / size << "\t";
            for ( int j = 0; j<m/4; j++ ) {
                std::cout << "*";
            }
            std:cout << "\n";
        }
    }

    void FFT::html() {

        std::cout <<  R""""(
            <!DOCTYPE html>
            <html>
            <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js"></script>
            <body>
            <canvas id="myChart" style="width:100%;max-width:700px"></canvas>

            <script>
            var xyValues = [
        )"""";

        double f;
        for ( int i=0; i<size/2+1; i++ ) {
            double a = out[i][0], b = out[i][1], m = sqrt(a*a+b*b) / size;            
            f = (int) (SAMPLE_RATE * 1.0 * i / size);
            std::cout << "{x: " << f << ", y:" << m << "},\n";
        }   

        std::cout << R"""(
            ];
            new Chart("myChart", {
            type: "scatter",
            data: {
                datasets: [{
                pointRadius: 4,
                pointBackgroundColor: "rgb(0,0,255)",
                data: xyValues
                }]
            },
            options: {
                legend: {display: false},
                scales: {
                   xAxes: [{ticks: {min: 0, max:44100/2}}],
                }                
            }
            });
            </script>
            </body>
            </html>
        )""";

    }

}

