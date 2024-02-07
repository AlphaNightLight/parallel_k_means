### Read the files
measure_folder = 'measures/';
plot_folder = 'plots/';

names = ['omp_static'; 'omp_dynamic'; 'mpi'; 'mpi_asynch'];
name_titles = ['OMP Static'; 'OMP Dynamic'; 'MPI'; 'MPI Asynchronous'];
tail = '_measures.csv';

report_serial = dlmread([measure_folder 'serial' tail], ',', 1, 0);
n_points_serial = report_serial(1, 1);
n_clusters_serial = report_serial(1, 2);
n_dimensions_serial = report_serial(1,3);
execution_time_serial = report_serial(1, 4);



for (i_type = 1:4)
  name = strtrim(names(i_type,:));
  name_title = strtrim(name_titles(i_type,:));
  report_parallel = dlmread([measure_folder name '_strong' tail], ',', 1, 0);
  num_cores = report_parallel(:,1);
  n_points = report_parallel(:, 2);
  n_clusters = report_parallel(1, 3);
  n_dimensions = report_parallel(1,4);
  execution_times = report_parallel(:,5);
  
  if (n_clusters != n_clusters_serial)
    disp(['Error: ' name '_strong and serial have different n_clusters!']);
    continue;
  endif
  
  if (n_dimensions != n_dimensions_serial)
    disp(['Error: ' name '_strong and serial have different n_dimensions!']);
    continue;
  endif
  
  if (!isequal( n_points, n_points(1,1) * ones(rows(n_points),1) ))
    disp(['Error: ' name '_strong does not have homogeneous n_points!']);
    continue;
  endif
  
  ### Strong Plot
  
  enlarged_execution_time_serial = execution_time_serial * ones(rows(execution_times),1);
  
  semilogx(num_cores,enlarged_execution_time_serial,'linewidth',1,...
           num_cores,execution_times,...
           'linewidth',1,'marker','.','markersize',10);
  
  xticks(num_cores);
  xticklabels(num_cores);
  grid on;
  set(gca,'XMinorTick','off');
  set(gca, 'xminorgrid', 'off');
  set(gca,'YMinorTick','off');
  set(gca, 'yminorgrid', 'off');
  
  title(['K-means ' name_title ', Strong Scaling']);
  ylabel('time [s]');
  xlabel('num\_cores [n]');
  
  legend('Serial','Parallel','location', 'northeast');
  print([plot_folder name '_strong_plot.png'],'-dpng');
  
  ### Speedup Plot
  
  speedup_factor = execution_times(1,1) * ones(rows(execution_times),1);
  speedup = speedup_factor ./ execution_times;
  
  semilogx(num_cores,num_cores,'linewidth',1,...
           num_cores,speedup,...
           'linewidth',1,'marker','.','markersize',10);
  
  xticks(num_cores);
  xticklabels(num_cores);
  grid on;
  set(gca,'XMinorTick','off');
  set(gca, 'xminorgrid', 'off');
  set(gca,'YMinorTick','off');
  set(gca, 'yminorgrid', 'off');
  
  title(['K-means ' name_title ', Speedup']);
  ylabel('speedup [n]');
  xlabel('num\_cores [n]');
  
  legend('Ideal','Parallel','location', 'northwest');
  print([plot_folder name '_speedup_plot.png'],'-dpng');
  
  ### Efficiency Plot
  
  efficiency = (speedup ./ num_cores) * 100 ;
  ideal_efficiency = 100 * ones(rows(execution_times),1);
  
  semilogx(num_cores,ideal_efficiency,'linewidth',1,...
           num_cores,efficiency,...
           'linewidth',1,'marker','.','markersize',10);
  
  xticks(num_cores);
  xticklabels(num_cores);
  grid on;
  set(gca,'XMinorTick','off');
  set(gca, 'xminorgrid', 'off');
  set(gca,'YMinorTick','off');
  set(gca, 'yminorgrid', 'off');
  
  title(['K-means ' name_title ', Efficiency']);
  ylabel('efficiency [%]');
  xlabel('num\_cores [n]');
  
  legend('Ideal','Parallel','location', 'northeast');
  print([plot_folder name '_efficiency_plot.png'],'-dpng');
  
endfor

for (i_type = 1:4)
  name = strtrim(names(i_type,:));
  report_parallel = dlmread([measure_folder name '_weak' tail], ',', 1, 0);
  num_cores = report_parallel(:,1);
  n_points = report_parallel(:, 2);
  n_clusters = report_parallel(1, 3);
  n_dimensions = report_parallel(1,4);
  execution_times = report_parallel(:,5);
  
  if (n_clusters != n_clusters_serial)
    disp(['Error: ' name '_weak and serial have different n_clusters!']);
    continue;
  endif
  
  if (n_dimensions != n_dimensions_serial)
    disp(['Error: ' name '_weak and serial have different n_dimensions!']);
    continue;
  endif
  
  powers_two = ( 2*ones(rows(n_points),1) ) .^ transpose(0 : rows(n_points)-1);
  if (!isequal( n_points, n_points(1,1) * powers_two ))
    disp(['Error: ' name '_weak does not have incremental n_points!']);
    continue;
  endif
  
  ### Weak Plot
  
  enlarged_execution_time_serial = execution_time_serial * ones(rows(execution_times),1);
  
  semilogx(num_cores,enlarged_execution_time_serial,'linewidth',1,...
           num_cores,execution_times,...
           'linewidth',1,'marker','.','markersize',10);
  
  xticks(num_cores);
  xticklabels(num_cores);
  grid on;
  set(gca,'XMinorTick','off');
  set(gca, 'xminorgrid', 'off');
  set(gca,'YMinorTick','off');
  set(gca, 'yminorgrid', 'off');
  
  title(['K-means ' name_title ', Weak Scaling']);
  ylabel('time [s]');
  xlabel('num\_cores [n] & n\_points\_scale\_ factor [n]');
  
  legend('Serial with size[1]','Parallel','location', 'northwest');
  print([plot_folder name '_weak_plot.png'],'-dpng');
  
endfor
