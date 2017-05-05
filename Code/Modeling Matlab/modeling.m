function varargout = modeling(varargin)
% MODELING MATLAB code for modeling.fig
%      MODELING, by itself, creates a new MODELING or raises the existing
%      singleton*.
%
%      H = MODELING returns the handle to a new MODELING or the handle to
%      the existing singleton*.
%
%      MODELING('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MODELING.M with the given input arguments.
%
%      MODELING('Property','Value',...) creates a new MODELING or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before modeling_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to modeling_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help modeling

% Last Modified by GUIDE v2.5 12-May-2016 22:36:58

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @modeling_OpeningFcn, ...
                   'gui_OutputFcn',  @modeling_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT



% --- Executes just before modeling is made visible.
function modeling_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to modeling (see VARARGIN)

% Choose default command line output for modeling
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes modeling wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = modeling_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on selection change in COM_number.
function COM_number_Callback(hObject, eventdata, handles)
% hObject    handle to COM_number (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns COM_number contents as cell array
%        contents{get(hObject,'Value')} returns selected item from COM_number


% --- Executes during object creation, after setting all properties.
function COM_number_CreateFcn(hObject, eventdata, handles)
% hObject    handle to COM_number (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in open_serial.
function open_serial_Callback(hObject, eventdata, handles)
% hObject    handle to open_serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global scom;
global ds;
global da;
global a;
global x;
global y;
global n_disk;
global bytesreceived;
global laser_x;
global laser_y;
global envi;
envi=[];
ds=1.02;
da=pi/40;
a=0;
x=[0];
y=[0];
laser_x=[0];
laser_y=[0];
n_disk=0;
bytesreceived=0;


% 若按下【打开串口】按钮，打开串口
if get(hObject, 'value')
    % 获取串口的端口名
    com_n=sprintf('com%d', get(handles.COM_number, 'value'));

    % 获取波特率
    rates = [2400 4800 9600 14400 19200 38400 57600 115200];
    baud_rate = rates(get(handles.Baud_rate, 'value'));

    % 创建串口对象
    scom = serial(com_n);
    % 配置串口属性
    scom.BaudRate=baud_rate;                    %设定波特率
    scom.Parity='none';                         %无奇偶校验
    scom.DataBits=8;                            %数据位8位
    scom.StopBits=1;                            %停止位1位
    %scom.InputBufferSize = 1024;                %输入缓冲区
    %scom.OutputBufferSize = 1024;               %输出缓冲区
    scom.Timeout=300;
    set(scom,'BytesAvailableFcnCount',1,'BytesAvailableFcnMode','byte','BytesAvailableFcn',{@bytes,handles});
    %,'TimerPeriod',0.01,'timerfcn',{@bytes,handles} 
    %set(handles.figure1, 'UserData', scom);
    % 尝试打开串口
    set(hObject,'value',1);
    try
        fopen(scom);  %打开串口
        set(handles.status,'String','Opend');
        %count=1;
        %cla(handles.plot);  %清除上一幅
       % picture_name=1;
    catch   % 若串口打开失败，提示“串口不可获得！”
        msgbox('无串口或被占用！');
        set(hObject, 'value', 0);  %弹起本按钮 
        set(handles.status,'String','Cannot open');
        set(hObject,'String','打开串口');
        return;
    end 
    set(hObject, 'String', '关闭串口');  %设置本按钮文本为“关闭串口”
else  %若关闭串口
       %停止并删除串口对象
        scoms = instrfind;
        stopasync(scoms);
        fclose(scoms);
        delete(scoms);
        set(hObject,'Value',0);
        set(hObject, 'String', '打开串口');  %设置本按钮文本为“打开串口” 
        set(handles.status,'String','Closed');
end
% Hint: get(hObject,'Value') returns toggle state of open_serial

function bytes(obj,eventdata,handles)
global ds;
global da;
global a;
global x;
global y;
global n_disk;
global carstatus;
global bytesreceived;
global laser_x;
global laser_y;
global envi;

n=get(obj,'BytesAvailable');
%display(n);
if n
    sa=fread(obj,1);
    
    c=char(sa);
    %display(dec2hex(c));
    bytereceived = bytesreceived + 1;
    if bytesreceived<=20
        
        if c>50
            switch(c)
                case 114  %r
                    carstatus='right';               
                case 103  %g
                    carstatus='go';              
                case 98  %b
                    carstatus='back'; 
            end
        else
            n_disk=double(c);
            switch (num2str(carstatus))
                case 'right'
                    a=a+n_disk.*da;
                case 'go'
                    dx=n_disk.*ds.*cos(a);
                    dy=n_disk.*ds.*sin(a);
                    x_new=x(end)+dx;
                    y_new=y(end)-dy;
                    x=[x x_new];
                    y=[y y_new];
                    %plot(handles.drawing,x,y);
                    %hold on;
                    %set(handles.drawing,'Xlim',[-300 300],'Ylim',[-300 300]);
                case 'back'
                    dx=n_disk.*ds.*cos(a);
                    dy=n_disk.*ds.*sin(a);
                    x_new=x(end)+dx;
                    y_new=y(end)-dy;
                    x=[x x_new];
                    y=[y y_new];
                    %plot(handles.drawing,x,y);
                    %hold on;
                    %set(handles.drawing,'Xlim',[-300 300],'Ylim',[-300 300]);
            end                
        end
    else
        
        buf=double(c);
        envi=[envi buf];
        if bytesreceived==1010
            bytesreceived=0;
            display(envi);
            for i=1:9:982
                b=a+(envi(i)-160).*4.*pi/180;
                l=envi(i+2).*25.6+envi(i+1).*0.1;
                bdx=l.*cos(b);
                bdy=l.*sin(b);
                laser_x_new=x(end)+bdx;
                laser_y_new=y(end)-bdy;
                laser_x=[laser_x laser_x_new];
                laser_y=[laser_y laser_y_new];
                
                b=b+pi/180;
                l=envi(i+4).*25.6+envi(i+3).*0.1;
                bdx=l.*cos(b);
                bdy=l.*sin(b);
                laser_x_new=x(end)+bdx;
                laser_y_new=y(end)-bdy;
                laser_x=[laser_x laser_x_new];
                laser_y=[laser_y laser_y_new];
                
                b=b+pi/180;
                l=envi(i+6).*25.6+envi(i+5).*0.1;
                bdx=l.*cos(b);
                bdy=l.*sin(b);
                laser_x_new=x(end)+bdx;
                laser_y_new=y(end)-bdy;
                laser_x=[laser_x laser_x_new];
                laser_y=[laser_y laser_y_new];
                
                b=b+pi/180;
                l=envi(i+8).*25.6+envi(i+7).*0.1;
                bdx=l.*cos(b);
                bdy=l.*sin(b);
                laser_x_new=x(end)+bdx;
                laser_y_new=y(end)-bdy;
                laser_x=[laser_x laser_x_new];
                laser_y=[laser_y laser_y_new];
            end
            envi=[];
            plot(handles.drawing,laser_x,laser_y,'.');
            set(handles.drawing,'Xlim',[-300 300],'Ylim',[-300 300]);
        end
                
            
    end

        
        
    %c_c=str2num(dec2hex(c'))';
    %set(handles.data,'string',[get(handles.data,'string') num2str(c_c) '  '])
    %set(handles.data,'string',[get(handles.data,'string') n_disk]);
end

% --- Executes on selection change in Baud_rate.
function Baud_rate_Callback(hObject, eventdata, handles)
% hObject    handle to Baud_rate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns Baud_rate contents as cell array
%        contents{get(hObject,'Value')} returns selected item from Baud_rate


% --- Executes during object creation, after setting all properties.
function Baud_rate_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Baud_rate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on key press with focus on open_serial and none of its controls.
function open_serial_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to open_serial (see GCBO)
% eventdata  structure with the following fields (see UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)



function data_Callback(hObject, eventdata, handles)
% hObject    handle to data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of data as text
%        str2double(get(hObject,'String')) returns contents of data as a double


% --- Executes during object creation, after setting all properties.
function data_CreateFcn(hObject, eventdata, handles)
% hObject    handle to data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in open_serial.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to open_serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in close_serial.
function close_serial_Callback(hObject, eventdata, handles)
% hObject    handle to close_serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in clean.
function clean_Callback(hObject, eventdata, handles)
% hObject    handle to clean (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global ds;
global da;
global a;
global x;
global y;
cla(handles.drawing,'reset');
x=[0];
y=[0];
a=0;
