classdef dtmf_generator < handle
    %DTMF_GENERATOR The App is a simple DTMF signal generator.
    %   Detailed explanation goes here
    
    properties
        Figure
        ButtonOne
        ButtonTwo
        ButtonThree
        ButtonFour
        ButtonFive
        ButtonSix
        ButtonSeven
        ButtonEight
        ButtonNine
        ButtonStar
        ButtonZero
        ButtonHashMark
    end
    
    methods
        function app = dtmf_generator
            % Main figure
            app.Figure = figure('Name', 'DTMF Generator', ...
                'MenuBar', 'none', ...
                'NumberTitle', 'off', ...
                'Position', [100 100 170 220], ...
                'CloseRequestFcn', @app.closeApp);
            app.ButtonOne = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '1', ...
                'FontSize', 14, ...
                'Position', [10, 160, 40, 40], ...
                'Callback', {@app.dtmf_event, [697 1209]});
            app.ButtonTwo = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '2', ...
                'FontSize', 14, ...
                'Position', [60, 160, 40, 40], ...
                'Callback', {@app.dtmf_event, [697 1336]});
            app.ButtonThree = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '3', ...
                'FontSize', 14, ...
                'Position', [110, 160, 40, 40], ...
                'Callback', {@app.dtmf_event, [697 1477]});
            app.ButtonFour = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '4', ...
                'FontSize', 14, ...
                'Position', [10, 110, 40, 40], ...
                'Callback', {@app.dtmf_event, [770 1209]});
            app.ButtonFive = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '5', ...
                'FontSize', 14, ...
                'Position', [60, 110, 40, 40], ...
                'Callback', {@app.dtmf_event, [770, 1336]});
            app.ButtonSix = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '6', ...
                'FontSize', 14, ...
                'Position', [110, 110, 40, 40], ...
                'Callback', {@app.dtmf_event, [770 1477]});
            app.ButtonSeven = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '7', ...
                'FontSize', 14, ...
                'Position', [10, 60, 40, 40], ...
                'Callback', {@app.dtmf_event, [852 1209]});
            app.ButtonEight = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '8', ...
                'FontSize', 14, ...
                'Position', [60, 60, 40, 40], ...
                'Callback', {@app.dtmf_event, [852 1336]});
            app.ButtonNine = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '9', ...
                'FontSize', 14, ...
                'Position', [110, 60, 40, 40], ...
                'Callback', {@app.dtmf_event, [852 1477]});
            app.ButtonStar = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '*', ...
                'FontSize', 14, ...
                'Position', [10, 10, 40, 40], ...
                'Callback', {@app.dtmf_event, [941 1209]});
            app.ButtonZero = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '0', ...
                'FontSize', 14, ...
                'Position', [60, 10, 40, 40], ...
                'Callback', {@app.dtmf_event, [941 1336]});
            app.ButtonHashMark = uicontrol(app.Figure, ...
                'Style', 'pushbutton', ...
                'String', '#', ...
                'FontSize', 14, ...
                'Position', [110, 10, 40, 40], ...
                'Callback', {@app.dtmf_event, [941 1477]});
        end
        
        function dtmf_event(app, hobject, eventdata, freq)
            fs = 8000;
            Ts = 1/fs;
            T = 500e-3;  %duration in s
            l = T/Ts;   %number of samples
            t = (0:l-1)*Ts;
            x = 0;
            for i = 1:length(freq)
                disp(freq(i));
                x = x + sin(2*pi*freq(i)*t);
            end
            
            soundsc(x, fs);
        end
        
        function closeApp(app, hobject, eventdata)
            delete(app.Figure);
        end
    end
    
end

