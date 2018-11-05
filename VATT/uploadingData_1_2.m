% open the VATT data
% May.17.2018 Heejoo Choi
% v 0.0.1 May 17 call all data and see the PSF
% v 0.0.2 may 18 for secondary data
% v 0.0.3 Make the video within certain area


dinfo = dir('*.fits'); % get the filder information

for N = 2:length(dinfo)
    filename = dinfo(N).name ; % get the name 
    dummyData = fitsread(filename,'Primary');
    fileNameTo = strcat(['shift : ',num2str(700-str2num(filename(43:45))),'um']);
    figure(1);
     threshold_gain=50;
     dummyData(dummyData<(threshold_gain*mean(dummyData(:))))=0;
     
    contour(real(log(dummyData(790:950,150:310))),15);colorbar;axis equal image;
%   contour(log(dummyData(700:1000,1:400)));colorbar;axis image;
%   imagesc(imcrop(dummyData,[1265 388 180 180]));colormap gray;colorbar;axis image;
    title(fileNameTo); pause(0.5);

%     saveas(gcf,strcat(num2str(N),'bmp'));%
    name = strcat([num2str(N),'.bmp']);
    saveas(gcf,name);%strcat(num2str(N),'bmp'));%

    
end

%% making video 
v = VideoWriter('VATT.avi','Uncompressed AVI');
v.FrameRate = 2;  % Default 30

open(v)
for N = 2:length(dinfo)
    name = strcat([num2str(N),'.bmp']);
    A = imread(name);
   writeVideo(v,A)
end

close(v)


%%
   dummyData(dummyData~=0)=1; 
   CC = bwconncomp(dummyData,4); %Find connected components in binary image
 [imageSizeY,imageSizeX] = size(dummyData)
   %%
   numPixl =cellfun(@numel,CC.PixelIdxList);% how many pixels are allocated to one particle
   %-----------------------------------[E3]-----------------------------------
   % Checking criteria for target/not-target and creating list of targets.
   ni = 1;
   for NN = 1:length(numPixl)
       if numPixl(NN) < 1000 % Maximun pixel size
           if numel(numPixl) > 0 % Minimum pixel size
               ptxlList = CC.PixelIdxList{NN};
               ptxlListX = rem(ptxlList,imageSizeY);
               ptxlListX(ptxlListX == 0) = imageSizeY;
               ptxlListY = floor(ptxlList/imageSizeY);
               mtxY(ni) = (mean(ptxlListY));
               mtxX(ni) = (mean(ptxlListX));
               mtxR(ni) = numPixl(NN);
               ni = ni + 1;
           else
               mtxY(ni) = 0;
               mtxX(ni) = 0;
               mtxR(ni) = 0;
           end
       end
   end
   StarMatirx = [mtxY' mtxX' mtxR'];
   
%    figure;plot(mtxY' mtxX' mtxR'
   
   %%
   
PossThre = 20;% percentage threshold
% StarMatirx(find(TempPoss < PossThre),:) = []; % remove low chance 
[a,~] = size(StarMatirx);
% mtxY = StarMatirx(:,1);  mtxX = StarMatirx(:,2) ;mtxR = StarMatirx(:,3);

[imageSizeY,imageSizeX] = size(dummyData);
TestI = dummyData;
% colorM = [1 0 0];

SampleImage = zeros(imageSizeY,imageSizeX,3);
ModulMin = min(TestI);
ModulMax = max(TestI - ModulMin);
NormalModul = (TestI-ModulMin)./(ModulMax);%.*Mask; % normalize modulation map and apply mask

% SampleImage(:,:,2) =  NormalModul;%./(max(NormalModul(:)));
SampleImage(:,:,2) =  h;%./(max(NormalModul(:)));
mtxR0 = mtxR;
if max(mtxR - min(mtxR(:))) ~= 0
    mtxR = (mtxR - min(mtxR(:)))./max(mtxR - min(mtxR(:)));
else mtxR = ones(size(mtxX'));end
% colorM = ones(length(mtxR),3);colorM(:,2) = (1-mtxR');
% colorM(:,1) = 1;colorM(:,3) = 0;

colorM = ones(length(mtxR),3);
colorM(:,2) = 1;
colorM(:,1) = 0;colorM(:,3) = 0;

SampleImage = insertShape(SampleImage, 'circle', [mtxY'  mtxX' ones(size(mtxX'))*10],'Color',colorM,'LineWidth',3);
% SampleImage2 = insertShape(SampleImage, 'circle', [mtxY'  mtxX' ones(size(mtxX'))*10],'Color',colorM,'LineWidth',3);
% 
% for ii=1:a
%    text_str{ii} = [num2str(StarMatirx(ii,3),'%0.0f') 'pxls '];% num2str(StarMatirx(ii,4),'%0.1f') '%'];
% end
% SampleImage3 = insertText(SampleImage2,[mtxY' mtxX'],text_str,'FontSize',18,...
%     'BoxOpacity',0,'TextColor','white');%num2str(PossM)

%%
figure(2);imagesc(SampleImage);axis image
figure(4);imagesc(SampleImage);axis image
figure(3);imagesc(TestI);axis image
   %%
   for NN = 1:length(numPixl)
               ptxlList = CC.PixelIdxList{NN};
               ptxlListX = rem(ptxlList,imageSizeY);
               ptxlListX(ptxlListX == 0) = imageSizeY;
               ptxlListY = floor(ptxlList/imageSizeY);
               mtxY(ni) = (mean(ptxlListY));
               mtxX(ni) = (mean(ptxlListX));
               mtxR(ni) = numPixl(NN);
               ni = ni + 1;
   end
clear