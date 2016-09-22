using MMF;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using TestForm.Properties;

namespace TestForm
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);
            Application.ThreadException += Application_ThreadException;
            Thread.GetDomain().UnhandledException += Application_UnhandledException;
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Form1 form = new Form1();
            MessagePump.Run(form);
            Settings.Default.Save();
        }

        private static void Application_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            StreamWriter writer = new StreamWriter("log.txt");
            writer.Write(e.ExceptionObject.ToString());
            writer.Flush();
            writer.Close();
        }

        static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e)
        {
            StreamWriter writer = new StreamWriter("log.txt");
            writer.Write(e.Exception.ToString());
            writer.Flush();
            writer.Close();
        }
    }
}
