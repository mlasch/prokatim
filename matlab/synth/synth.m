fs = 44100;
f_a1 = 440;  % frequency of tone a
f_c1 = f_a1 * 2^(-9/12);
f_d1 = f_a1 * 2^(-7/12);
f_e1 = f_a1 * 2^(-5/12);
f_f1 = f_a1 * 2^(-4/12);
f_g1 = f_a1 * 2^(-2/12);
f_h1 = f_a1 * 2^(2/12);
f_c2 = f_a1 * 2^(3/12);
f_d2 = f_d1 * 2;

tones = [f_d1 f_g1 f_g1 f_a1 f_h1 f_g1 f_d2 ... 
    f_h1 f_h1 f_c2 f_d2 f_c2 f_h1 f_c2 f_d2 ...
    f_a1 f_g1 f_a1 f_h1 f_a1];
duration = [2 2 1 1 2 2 4 3 1 2 1 1 1 1 2 1 1 1 1 2];
scale = 1/4;

y = [];

for k = 1:length(tones)
    N = 0:(scale*fs*duration(k))-1;
    w = 2*pi/fs*tones(k);
    adsr = adsr_profile(length(N), 0.2, 0.3, 0.4, 0.95, 0.8);
    harmonics = 5;
    s = zeros(1, length(N));
    
    for kk = 1:harmonics
        a = 0.9;
        b = exp(-a*(kk-1));
        s = s + b.*sin(kk*w*N);
    end
    
    s = s.*adsr;
    y = [y s];
end


soundsc(y, fs, 16);