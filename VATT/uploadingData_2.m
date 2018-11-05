% open the VATT data
% May.17.2018 Heejoo Choi
% v 0.0.1 May 17 call all data and see the PSF
% v 0.0.2 may 18 for secondary data
% v 0.0.4 Tru to find the vector from stat geometrical center and
% photometic center
%%
clear all;close all;clc
% load the  images
dinfo = dir('*.fits'); % get the filder information
ImageArray = cell(length(dinfo));
titleArray = cell(length(dinfo));

%%
for N = 2:length(dinfo)
    filename = dinfo(N).name ; % get the name 
    dummyData = fitsread(filename,'Primary');
    fileNameTo = strcat(['shift : ',num2str(700-str2num(filename(43:45))),'um']);
    ImageArray{N} = dummyData;
    titleArray{N} = fileNameTo;
%     figure(1);
%     imagesc(real(log(dummyData)));colorbar;axis equal image;colormap gray
%     title(fileNameTo); pause(0.5);    

%      threshold_gain=50;
%      dummyData(dummyData<(threshold_gain*mean(dummyData(:))))=0;
%        h=contour(real(log(dummyData(790:950,150:310))),15);colorbar;axis equal image;     
%       contour(log(dummyData(700:1000,1:400)));colorbar;axis image;
%       imagesc(imcrop(dummyData,[1265 388 180 180]));colormap gray;colorbar;axis image;
end

%% image process
threshold_gain=10;
% [imageSizeY,imageSizeX] = size(ImageArray{2})

GaussianCenter = zeros(1,length(dinfo));

%% for N = 2:2
for N = 2:length(dinfo)
%     testImage = imcrop(ImageArray{N},[150 790 160 160]);%:950,150:310;
    testImage = (ImageArray{N});%:950,150:310;
    testImage0 = testImage;
    [imageSizeY,imageSizeX] = size(testImage);

%     logTestImage = real(log(testImage));
    
    testImage(testImage<(threshold_gain*mean(testImage(:))))=0;
    testImage(testImage ~= 0) = 1;
    
%     figure(1);
% %     contour(real(log(testImae)));colorbar;axis equal image;
%     imagesc(testImage);colorbar;axis image;colormap gray
%     title(titleArray{N}); pause(0.5);
    
    CC = bwconncomp(testImage,4); %Find connected components in binary image
    numPixl =cellfun(@numel,CC.PixelIdxList);% how many pixels are allocated to one particle
    
    clear StarMatirx mtxY mtxX mtxR MaxInX MaxInY;    
    
    ni = 1;
   for NN = 1:length(numPixl)
       if numPixl(NN) < 10000 % Maximun pixel size
           if numel(numPixl) > 0 % Minimum pixel size
               ptxlList = CC.PixelIdxList{NN};
               ptxlListY = rem(ptxlList,imageSizeY);
               ptxlListY(ptxlListY == 0) = imageSizeY;
               ptxlListX = floor(ptxlList/imageSizeY) + 1;
               IntensityM = zeros(length(ptxlList),1);
               
               for MM = 1:length(ptxlList);
                   IntensityM(MM) = testImage0(ptxlListY(MM),ptxlListX(MM));
               end
               MaxIn = find(IntensityM == max(IntensityM));
               MaxInY(ni) = mean(ptxlListY(MaxIn));
               MaxInX(ni) = mean(ptxlListX(MaxIn));
               mtxY(ni) = (mean(ptxlListY));
               mtxX(ni) = (mean(ptxlListX));
               mtxR(ni) = numPixl(NN);
               ni = ni + 1;
           else
%                mtxY(ni) = 0;
%                mtxX(ni) = 0;
%                mtxR(ni) = 0;
           end
       end
   end
   StarMatirx = [mtxX' mtxY' MaxInX' MaxInY' mtxR'];  
   StarMatirx(StarMatirx(:,5) < 3,:) = [];
   [X,Y] = meshgrid(1:imageSizeX,1:imageSizeY);
   blankInX = zeros(imageSizeY,imageSizeX);
   blankInY = zeros(imageSizeY,imageSizeX);
   for MN = 1:length(StarMatirx(:,1))
       blankInX(round(StarMatirx(MN,1)),round(StarMatirx(MN,2))) = (StarMatirx(MN,3) - StarMatirx(MN,1));%/(StarMatirx(MN,3))^2;
       blankInY(round(StarMatirx(MN,1)),round(StarMatirx(MN,2))) = (StarMatirx(MN,4) - StarMatirx(MN,2));%/(StarMatirx(MN,3))^2;
   end
   
    xSign = zeros(size(StarMatirx,1),1);
    xSign((StarMatirx(:,1) - StarMatirx(:,3))>=0) = 1;
    xSign(xSign == 0) = -1;
    
    ySign = ones(size(StarMatirx,1),1);
%     ySign((StarMatirx(:,2) - StarMatirx(:,4))>=0) = 1;
%     ySign(ySign == 0) = -1;
    
    SignM = xSign.*ySign;
       
   Xdevia = ((StarMatirx(:,3) - StarMatirx(:,1)));
   Ydevia = ((StarMatirx(:,4) - StarMatirx(:,2)));
  
   MeanDevi = sum(sqrt(Xdevia.^2 + Ydevia.^2))./(length(numPixl));
%    MeanDevi = sum((Xdevia + Ydevia))./(length(numPixl));
%% 
   figure(1);
   subplot(1,3,1)
   imagesc(real(log(testImage0)));axis image;colormap gray;colorbar
   hold on 
   quiver(Y,X,blankInY,blankInX,200,'y','LineWidth',1,'MarkerSize',10); 
   title(strcat({titleArray{N},['Avg deviation = ',num2str(MeanDevi)]})); 
   hold off
   subplot(1,3,2)
   histfit(-Xdevia,10);axis([-5 5 0 10]);grid on;
   hstf = fitdist(-Xdevia,'normal');
   GauCnt = hstf.mu;   
   title(strcat({['Deviation amplitude: x axis'],['Gaussian Fit center : ',num2str(GauCnt)]}));
   GaussianCenter(N)=GauCnt; % 
  subplot(1,3,3)
  h=polarhistogram(atan2(Ydevia,-Xdevia),20);
%   hstfA = fitdist(h.Values','normal');
%   theta1 = (h.BinEdges(1:end-1) + h.BinEdges(2:end))/2;   % bin centers
%   y = pdf(hstfA,theta1);y = (y-min(y))./max((y-min(y)))*max(h.Values')   ;
% hold on
    GaussianACenter(N)=atan2(mean(Ydevia),mean(-Xdevia))*180/pi; %
   title(strcat({['Deviation angle polar histogram']}));%,['Gaussian Fit center : ',num2str(GaussianACenter(N))]}));
% polarplot(theta1,y,'r')
% hold off
      disp('Diplay is done')


   name = strcat([num2str(N),'.bmp']);
   saveas(gcf,name);%strcat(num2str(N),'bmp'));%

   
end

%% making video 
v = VideoWriter('VATT_quiver_histogram.avi','Motion JPEG AVI');
v.FrameRate = 2;  % Default 30

open(v)
for N = 2:length(dinfo)
    name = strcat([num2str(N),'.bmp']);
    A = imread(name);
   writeVideo(v,A)
end

close(v)



