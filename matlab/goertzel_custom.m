function [ X ] = goertzel_custom(x, f0, fs)
%GOERTZEL_CUSTOM Implementation of the goertzel algorithm
%   The function takes
%   x <- sampled data
%   f0 <- frequency to be detected
%   fs <- sample frequency of the data samples

    N = length(x);
    m = N*f0/fs;
    
    cos_const = cos(2*pi*m/N);
    %sin_const = sin(2*pi*m/N);
    coeff = 2 * cos_const;
    
    sprev1 = 0;
    sprev2 = 0;
    
    for n = 1:N-1
        s = x(n) + coeff .* sprev1 - sprev2;
    
        sprev2 = sprev1;
        sprev1 = s;
    end
    
    X = sprev1^2 + sprev2^2 - sprev1*sprev2*coeff;
    X = 10*log(X);
end