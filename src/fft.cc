// 
// YASE FFT Module Implementation
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

#include "fft.hh"
#include "yase.hh"

namespace yase {

    //! Construct an FFT module with the given size.
    //! \param size The number of samples to transform
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

    //! Get the FFT data. 
    //! This method has the side effect of resetting the FFT so that it starts over.
    fftw_complex * FFT::get() {
      n = 0;
      return out;
    }

    //! Print the FFT data to stdout
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

    //! Print the FFT data as a json object to stdout
    std::string FFT::json() {

        string result = "[";
        double f;

        for ( int i=0; i<size/2+1; i++ ) {
            double a = out[i][0], b = out[i][1], m = sqrt(a*a+b*b) / size;            
            f = (int) (SAMPLE_RATE * 1.0 * i / size);
            result += "{\"x\": " + 
                      std::to_string(f) + 
                      ", \"y\":" +
                      std::to_string(m) 
                      + "}";
            if ( i < size/2+1-1 ) {
                result += ",";
            }
        }     

        result += "]";
        return result;

    }

    //! Print a nice plot of the FFT to stdout that can be viewed with a web browser. 
    void FFT::html() {

        std::cout <<  R""""(
            <!DOCTYPE html> 
            <html> 
            <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js"></script>
            <body background='#000'>
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
                title: {
                    display: true,
                    text: 'YASE Spectrum'
                },
                scales: {
                   xAxes: [{
                     ticks: {min: 0, max:44100/2},
                     scaleLabel: {
                       labelString: "Frequency (Hz)",
                       display: true
                     }
                   }],
                   yAxes: [{
                     scaleLabel: {
                       labelString: "Magnitude",
                       display: true
                     }
                   }]                   
                }                
            }
            });
            </script>
            </body>
            </html>
        )""";

    }

}