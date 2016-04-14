clc;                      %clear the command window before displaying anything
clear all;                %clear all variables in the current workspace
s2 = serial('COM14', 'BaudRate', 9600, 'Parity', 'none', 'DataBits', 8, 'StopBits', 1, 'FlowControl', 'none');                  %initialize the communication with arduino to get values from the serial port
fopen(s2);                %opens files and obtains information from file


x = 0;                     %initialize the variable to be used a loop

while(x<200)
    x = x + 1;                  %increment the variable
    y1(x) = fscanf(s2,'%f');    %reading the value of the first data(temperature) in real time
    y2(x) = fscanf(s2,'%f');    %reading value for the second data(light-level) in real time
    y3(x) = fscanf(s2,'%f');    %reading the value for the third(humidity) data in real time
    
    
    drawnow;                   %flushing the event queue and updating the figure window
    
    %USING THE SUBPLOT FUNCTION TO CREATE THREE PLOTS ON ONE WINDOW
    
    %Plot the sketch for the temperature values
    
    subplot(2,2,1);                    %set the position for the temperature graph(top right corner)
    plot (y1,'b','linewidth',1);       %settings for the line used to plot the values
    title('temp');                     %title of the temperature graph
    xlabel('time/s');                  %Label of the horizontal axis of the graph and units
    ylabel('*C');                      %label of the vertical axis of the graph(in this case units of temperature)
    
    grid on;                           %display the major grid lines for the current axes
    hold on;                           %retains the current plots so that they are not deleted when displaying other plots
    
    
    %Plot the sketch for the light values
    subplot(2,2,2);                         %set the position for the light graph(top left corner)
    plot (y2,'g','linewidth',1);            %settings for the line used to plot the values
    title('light');                         %title of the light graph
    xlabel('time/s');                       %Label of the horizontal axis of the graph and units
    ylabel('Lux');                          %label of the vertical axis of the graph(in this case units of light)
    
    grid on;
    hold on;
    
    %Plot the sketch for the humidity values
    
    subplot(2,3,5);                   %set the position for the humidity graph(down in the middle)
    plot (y3,'r','linewidth',1);      %settings for the line used to plot the values
    title('humid');                   %title of the humidity graph
    xlabel('time/s');                 %Label of the horizontal axis of the graph and units
    ylabel('%');                      %label of the vertical axis of the graph(in this case units of light)
    
    grid on;
    hold on;
    
    pause(0.1);                    %pauses the execution for 0.1 seconds before continuing
end                                %terminates the while loop indicating end of input values
fclose(s2);                        %closes the open file that was in use
delete(s2);                        %delete the graphics of the object s2