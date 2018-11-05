% open the VATT data
% May.17.2018 Heejoo Choi
% v 0.0.1 May 17 call all data and see the PSF
% v 0.0.2 may 18 for secondary data


dinfo = dir('*.fits'); % get the filder information

for N = 2:2 %length(dinfo)
    filename = dinfo(N).name ; % get the name 
    dummyData = fitsread(filename,'Primary');
    fileNameTo = strcat(['shift : ',num2str(700-str2num(filename(43:45))),'um']);
    figure(1);
     threshold_gain=1.7;
     dummyData(dummyData<(threshold_gain*mean(dummyData(:))))=0;
     contour(real(log(dummyData)));colorbar;axis image;
%   contour(log(dummyData(700:1000,1:400)));colorbar;axis image;
%   imagesc(imcrop(dummyData,[1265 388 180 180]));colormap gray;colorbar;axis image;
    title(fileNameTo); pause(0.05);
end

clear