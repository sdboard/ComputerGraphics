function newpts = shiftAndScale(Arr)
curLetter = input('current letter: ','s');
shifted = [[]];
for i = 1:size(Arr,2)
    
%     x = (Arr(1,i));
%     y = (Arr(2,i));
    
    x = (250 - Arr(1,i)) / 250;
    y = (Arr(2,i) - 250) / 250;
    
    p = [-x,-y];
    shifted = [shifted;p];
end
newpts = shifted;
 str ='    ';
 str1 = strcat('vector<Coordinate> points',curLetter, '= {');
 
 it = 0;
 for j = 1:size(shifted)
     
     str = strcat(str,'Coordinate c',curLetter,num2str(it),' = Coordinate(',num2str(shifted(j,1)),'f, ',num2str(shifted(j,2)),'f);^');
     %str = {str;str2};
     %str = sprintf(str,'\n');
     if (it >= 1)
        str1 = strcat(str1,', c',curLetter,num2str(it));
     else
        str1 = strcat(str1,'c',curLetter,num2str(it));
     end
     it = it + 1;
     
 end
 newstr1 = strcat('vector<Coordinate> newPoints',curLetter,' = recursive_call(points',curLetter,');');
 str1 = strcat(str1,'};');
 C = strsplit(str,'^');
 outstr = '';
 for l = 1:size(C,2)
     outstr = [outstr char(10) C(1,l)];
 end
 %fprintf(outstr(:))
 filePh = fopen('pointspoints_.txt','w');
 [rows,cols]=size(outstr);
 for r=1:cols
    fprintf(filePh,outstr{1,r});
 end
 fprintf(filePh,str1);
 fprintf(filePh,'\n');
 fprintf(filePh,newstr1);
 fclose(filePh);
 
 
