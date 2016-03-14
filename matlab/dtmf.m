fs = 8000;  %sample rate 8khz
Ts = 1/fs;

l = 108;   %number of samples 2*fs = 2s

t = (0:l-1)*Ts;   %timebase

f1 = 697;
f2 = 1336;

A1 = 1;
A2 = 1;

signal = A1.*t;

% generate the signal
signal = A1*sin(2*pi*f1*t) ...
    + A2*sin(2*pi*f2*t);

signal = awgn(signal,10);   % add noise

% hamming window
hamm = hamming(length(signal));
signal = signal.*hamm.';

row = [590 697 770 852 941];
col = [1209 1336 1477];

X = 1:length(row);

for i = 1:length(row)
    X(i) = goertzel_custom(signal, row(i), fs);
    disp(X(i))
end

stem(row, X);