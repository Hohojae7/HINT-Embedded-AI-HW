t=out.simout.time;
y1=out.simout.data(:,1);
y2=out.simout.data(:,2);

plot(t,y1,t,y2,':');
xlabel('Time(s)');
ylabel('Magnitude');
legend('sine wave', 'integration', 'Location', 'SW');