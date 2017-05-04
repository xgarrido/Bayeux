// test_signal_triangle_signal_shape.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// Third party:
// - Bayeux/datatools:
#include <datatools/clhep_units.h>
#include <datatools/temporary_files.h>
#include <datatools/utils.h>
#include <datatools/clhep_units.h>
// - Bayeux/mygsl:
#include <mygsl/parameter_store.h>
// - Bayeux/geomtools:
#include <geomtools/geomtools_config.h>
#include <geomtools/gnuplot_draw.h>
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
#include <geomtools/gnuplot_i.h>
#include <geomtools/gnuplot_drawer.h>
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY

// This project:
#include <mctools/signal/triangle_signal_shape.h>

int main (int argc_, char ** argv_)
{
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for class 'mctools::signal::triangle_signal_shape'!" << std::endl;

    bool draw = false;

    int iarg = 1;
    while (iarg < argc_) {
      std::string arg = argv_[iarg];
      if (arg == "-D" || arg == "--draw") draw = true;
      iarg++;
    }

    static const double nVs = CLHEP::volt * CLHEP::nanosecond;
    static const double mV  = 1e-3 * CLHEP::volt;
    mygsl::parameter_store ps;
    ps.add("q", datatools::TYPE_REAL, "Q");
    ps.add("t0", datatools::TYPE_REAL, "T0");
    // Set the values of the parameters in the store:
    ps.grab("q").set_real_value(3.0 * nVs);
    ps.grab("t0").set_real_value(10.0 * CLHEP::ns);
    ps.tree_dump(std::clog, "Parameter store: ");


    mctools::signal::triangle_signal_shape tss1;
    tss1.set_polarity(mctools::signal::POL_NEGATIVE);
    tss1.set_rise_time(8.0 * CLHEP::nanosecond);
    tss1.set_fall_time(20.0 * CLHEP::nanosecond);
    tss1.register_requested_parameter("q", datatools::TYPE_REAL);
    tss1.register_requested_parameter("t0", datatools::TYPE_REAL);
    tss1.set_parameter_store(ps);
    tss1.initialize_simple();

    mctools::signal::triangle_signal_shape tss2;
    tss2.set_polarity(mctools::signal::POL_NEGATIVE);
    tss2.set_rise_time(28.0 * CLHEP::nanosecond);
    tss2.set_fall_time(58.0 * CLHEP::nanosecond);
    tss2.register_requested_parameter("q", datatools::TYPE_REAL);
    tss2.register_requested_parameter("t0", datatools::TYPE_REAL);
    tss2.set_parameter_store(ps);
    tss2.initialize_simple();

    datatools::temp_file tmp_file;
    tmp_file.set_remove_at_destroy(true);
    tmp_file.create("/tmp", "test_signal_triangle_signal_shape_");

    tmp_file.out() << "#signal_1:\n";
    tss1.write_ascii(tmp_file.out(),
                     0.0 * CLHEP::nanosecond,
                     300.0 * CLHEP::nanosecond,
                     1000);
    tmp_file.out() << "\n\n";
    tmp_file.out() << "#end_signal_1\n";

    // Change the values of the parameters in the store:
    ps.grab("q").set_real_value(3.2 * nVs);
    ps.grab("t0").set_real_value(65.0 * CLHEP::ns);

    tmp_file.out() << "#signal_2:\n";
    tss2.write_ascii(tmp_file.out(),
                     0.0 * CLHEP::nanosecond,
                     300.0 * CLHEP::nanosecond,
                     1000);
    tmp_file.out() << "\n\n";
    tmp_file.out() << "#end_signal_2\n";
    tmp_file.out() << std::flush;

    // Change the values of the parameters in the store:
    ps.grab("q").set_real_value(4.2 * nVs);
    ps.grab("t0").set_real_value(15.0 * CLHEP::ns);

    tmp_file.out() << "#signal_1:\n";
    tss1.write_ascii(tmp_file.out(),
                     0.0 * CLHEP::nanosecond,
                     300.0 * CLHEP::nanosecond,
                     1000);
    tmp_file.out() << "\n\n";
    tmp_file.out() << "#end_signal_1\n";

    // Change the values of the parameters in the store:
    ps.grab("q").set_real_value(1.5 * nVs);
    ps.grab("t0").set_real_value(35.0 * CLHEP::ns);

    tmp_file.out() << "#signal_2:\n";
    tss2.write_ascii(tmp_file.out(),
                     0.0 * CLHEP::nanosecond,
                     300.0 * CLHEP::nanosecond,
                     1000);
    tmp_file.out() << "\n\n";
    tmp_file.out() << "#end_signal_2\n";
    tmp_file.out() << std::flush;

    if (draw) {
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
      Gnuplot g1;
      g1.cmd("set title 'Test mygsl::signal::triangle_signal_shape' ");
      g1.cmd("set grid");
      g1.cmd("set xrange [0:400]");
      g1.cmd("set yrange [-500:+50]");
      g1.set_xlabel("time (ns)").set_ylabel("Signal (mV)");

      {
        std::ostringstream plot_cmd;
        plot_cmd << "plot '" << tmp_file.get_filename() << "' "
                 << " index 0 using (column(1)/" << CLHEP::nanosecond
                 << "):(column(2)/" << mV << ')'
                 << " title 'Signal shape 1' with lines lw 4";
        plot_cmd << ", '' "
                 << " index 1 using (column(1)/" << CLHEP::nanosecond
                 << "):(column(2)/" << mV << ")"
                 << " title 'Signal shape 2' with lines lw 2";
        g1.cmd(plot_cmd.str());
        g1.showonscreen(); // window output
        geomtools::gnuplot_drawer::wait_for_key();
        usleep(200);
      }

      {
        std::ostringstream plot_cmd;
        plot_cmd << "plot '" << tmp_file.get_filename() << "' "
                 << " index 2 using (column(1)/" << CLHEP::nanosecond
                 << "):(column(2)/" << mV << ')'
                 << " title 'Signal shape 1' with lines lw 4";
        plot_cmd << ", '' "
                 << " index 3 using (column(1)/" << CLHEP::nanosecond
                 << "):(column(2)/" << mV << ")"
                 << " title 'Signal shape 2' with lines lw 2";
        g1.cmd(plot_cmd.str());
        g1.showonscreen(); // window output
        geomtools::gnuplot_drawer::wait_for_key();
        usleep(200);
      }

#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
    }

    std::clog << "The end." << std::endl;
  }
  catch (std::exception & x) {
    std::cerr << "error: " << x.what () << std::endl;
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    std::cerr << "error: " << "unexpected error!" << std::endl;
    error_code = EXIT_FAILURE;
  }
  return (error_code);
}