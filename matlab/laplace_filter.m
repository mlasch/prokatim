fs = 8000;  %sample rate 8khz
Ts = 1/fs;


l = 2*fs;   %number of samples 2*fs = 2s

t = (0:l-1)*Ts;   %timebase

f1 = 300;
f2 = 400;

f3 = 500;


A1 = 3;
A2 = 2;
A3 = 1.1;

% generate the signal

signal = A1*sin(2*pi*f1*t) ...
    + A2*sin(2*pi*f2*t) ...
    + A3*sin(2*pi*f3*t);


signal = awgn(signal,10);   % add noise

%stem(t,signal);

nfft = 2^nextpow2(l);
y = fft(signal, nfft) / l;
f = fs/2*linspace(0,1,nfft/2+1);

plot(f,2*abs(y(1:nfft/2+1)));

