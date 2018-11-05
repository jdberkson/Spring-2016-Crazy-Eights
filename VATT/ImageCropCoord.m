function Output = ImageCropCoord(Inputnum,Boundary)
% this functin return the porper number to crop the image.
% negative starting and over the size coordiante recalculate
% july 17 2018 Heejoo Choi
switch nargin
    case 1
        for N = 1:length(Inputnum)
        if Inputnum <= 0
            Output(N) = 1;
        else
            Output(N) = Inputnum(N);
        end
        end
    case 2
        for N = 1:length(Inputnum)
        if Inputnum(N) >= Boundary;
            Output(N) = Boundary;
        else
            Output(N) = Inputnum(N);
        end
        end
end
end