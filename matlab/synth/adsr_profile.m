function [ y ] = adsr_profile(N, ta, td, ts, tr, ED, ES)
%ADSR_PROFILE Creates an ADSR envelope
%   N   :   Number of samples
%   ta  :   Attack time
%   td  :   Decay time
%   ts  :   Sustain time
%   tr  :   Release time
%   y   :   Envelope signal

% Attack phase
Na = floor(N*ta);
ya = linspace(0, 1, Na);

% Decay phase
Nd = floor(N*td);
yd = linspace(1, ED, Nd);

% Sustain phase
Ns = floor(N*ts);
ys = linspace(ED, ES, Ns);

% Release phase
Nr = floor(N-Na-Nd-Ns);
yr = linspace(ES, 0, Nr);

y = [ya yd ys yr];

%n = 0:N-1;
%stem(n, y);
end