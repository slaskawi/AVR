/* ====================================================================
*
* The Apache Software License, Version 1.1
*
* Copyright (c) 2003 Validity Systems Inc.  All rights
* reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* 3. The end-user documentation included with the redistribution, if
*    any, must include the following acknowlegement:
*       "This product includes software developed by the
*        Validity Systems Inc."
*    Alternately, this acknowlegement may appear in the software itself,
*    if and wherever such third-party acknowlegements normally appear.
*
* 4. The names "#Logger", "CS Logger", and "Validity Systems
*    Inc." must not be used to endorse or promote products derived
*    from this software without prior written permission. For written
*    permission, please contact apache@apache.org.
*
* 5. Products derived from this software may not be called "#Logger"
*    nor may "#Logger" appear in their names without prior written
*    permission of Validity Systems Inc.
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
* ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
* USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* ====================================================================
*
* This software consists of voluntary contributions made by many
* individuals on behalf of the Validity Systems Inc.
*
*
*/

/*
 *  Author: Athony La Forge (Validity Systems Inc.) 
 */

using System;
using System.Collections;
using System.Threading;
using System.IO;

namespace VS.Logger
{
    /// <summary>
    /// Represents a log message
    /// </summary>
    public struct LoggerMessage
    {
        public int level;
        public string level_desc;
        public string tag;
        public string message;
        public long time;
    }

    /// <summary>
    /// Abstract class responsible managing (in the form or a queue) and dispatching log messages
    /// to their appropriate source.  The basic premises is that everything from a file writers, 
    /// to an e-mailer, to an syslog interface, to a db interface, etc... could be implemented and the only
    /// thing the implementer would need to worry about was the interface.
    /// </summary>
    public abstract class LoggerEventHandler
    {
        private bool alive = false;
        private Queue q = null;
        private Thread dispatch = null;
        protected string[] logLevelDescriptors = null;

        public LoggerEventHandler()
        {
            q = Queue.Synchronized(new Queue(1000));
            start();
        }

        /// <summary>
        /// Starts the execution of the Handler (e.g. Queue goes live)
        /// </summary>
        public void start()
        {
            //It's already alive, nothing to do
            if (alive) return;

            //Make sure this is the only place where alive is set to true
            alive = true;
            dispatch = new Thread(new ThreadStart(dispatchMessages));
            dispatch.Start();
        }

        /// <summary>
        /// Stops the execution of the Handler gracefully (e.g. everything in the queue is
        /// dispatched, but nothing can be added to it)
        /// </summary>
        public void shutdown()
        {
            //Nothing to do
            if (!alive) return;

            alive = false;
            Monitor.Enter(q);
            Monitor.PulseAll(q);
            Monitor.Exit(q);
        }

        /// <summary>
        /// Will immediately shutdown the thread without cleaning up or clearing
        /// out the queue.  Consequently this is not the recommended way to terminate.
        /// </summary>
        public void abort()
        {
            if (!alive) return;

            alive = false;
            dispatch.Abort();
        }

        /// <summary>
        /// This is the worker method where the messages get dispatched
        /// </summary>
        protected void dispatchMessages()
        {
            //int max = 0;
            while (alive)
            {
                while ((q.Count != 0) && alive)
                {
                    //if(q.Count > max)max = q.Count;
                    log((LoggerMessage)q.Dequeue());
                }

                if ((alive) && (q.Count == 0))
                {
                    Monitor.Enter(q);
                    if (q.Count == 0) Monitor.Wait(q);
                    Monitor.Exit(q);
                }
            }

            //You will only reach this code if you are exiting the program
            //This block ensures the messages that were queued up will get dumped
            while (q.Count != 0)
            {
                log((LoggerMessage)q.Dequeue());
            }

            //Do any special shutdown you need to do
            onShutdown();

            //Remove thread reference so you don't have object leak
            dispatch = null;
        }

        /// <summary>
        /// Responsible for queuing the log message
        /// </summary>
        /// <param name="tag"></param>
        /// <param name="level"></param>
        /// <param name="level_desc"></param>
        /// <param name="message"></param>
        public void log(string tag, int level, string level_desc, string message)
        {
            if (!alive) return;

            LoggerMessage lm = new LoggerMessage();
            lm.message = message;
            lm.tag = tag;
            lm.level = level;
            lm.level_desc = level_desc;
            lm.time = System.DateTime.Now.ToFileTime();

            q.Enqueue(lm);

            Monitor.Enter(q);
            Monitor.PulseAll(q);
            Monitor.Exit(q);
        }

        /// <summary>
        /// User implementable log method (e.g. what actions they need to take to perform
        /// a "log" operation, like logging to a file for example).
        /// </summary>
        /// <param name="message"></param>
        protected abstract void log(LoggerMessage message);

        /// <summary>
        /// User implementable, meant for cleanup when the thread is stopped (e.g. if you need to
        /// close files, db connections, etc...)
        /// </summary>
        protected abstract void onShutdown();
    }


    /// <summary>
    /// Test class, by no means a unit test, but more for experimenting
    /// </summary>
    class Class1
    {
        //[STAThread]
        //static void Main(string[] args)
        //{
        //Logger logger = new Logger(3, "test.txt");
        //	Logger logger = Logger.singleton();
        //	logger.addSpecialLogger(2, "test2.txt");


        //	for(int x = 0; x < 100; x++)
        //	{
        //		for(int i = 0; i < 100; i++)
        //		{
        //			logger.log(i%6, "TEST", "message " + i);
        //		}
        //Thread.Sleep(500);
        //	}
        //	System.Console.WriteLine("{0}",DateTime.Now.ToFileTime()/10000);
        //			logger.log(0, "TEST", "message 1");
        //			logger.log(1, "TEST", "message 2");
        //			logger.log(2, "TEST", "message 3");
        //System.Console.ReadLine();
        //	logger.shutdown();
        //}
        //}

        /// <summary>
        /// Simple To File Logger.  All messages a written out to a flat text file.
        /// </summary>
        public class BasicFileLogEventHandler : LoggerEventHandler
        {
            StreamWriter stream = null;
            bool append = true;

            public BasicFileLogEventHandler(String filename)
            {
                if (filename != null)
                {
                    FileMode fm;

                    if (append) fm = FileMode.Append;
                    else fm = FileMode.Create;

                    FileStream fs = new FileStream(filename, fm, FileAccess.Write, FileShare.Read);

                    //file = new StreamWriter(filename);

                    stream = new StreamWriter(fs, System.Text.Encoding.UTF8, 4096);
                    //stream = new StreamWriter(fs, System.Text.Encoding.UTF8, 1024);
                }
            }

            override protected void log(LoggerMessage message)
            {
                if (stream == null) return;
                string time = System.DateTime.FromFileTime(message.time).ToString();
                string test = "[" + time + " [" + message.level + ":" + message.level_desc + " (" + message.tag + ")] " + message.message + " ]\r\n";
                stream.Write("[" + time + " [" + message.level + ":" + message.level_desc + " (" + message.tag + ")] " + message.message + " ]\r\n");
            }

            protected override void onShutdown()
            {
                if (stream != null)
                {
                    stream.Flush();
                    stream.Close();
                }
            }

            /// <summary>
            /// Flag to append the text file.  If this flag is not set it will overwrite.
            /// </summary>
            /// <param name="flag"></param>
            public void setAppend(bool flag)
            {
                this.append = flag;
            }

            /// <summary>
            /// Get's append flag
            /// </summary>
            /// <returns></returns>
            public bool getAppend()
            {
                return append;
            }
        }

        /// <summary>
        /// Primary class responsbile for logging.  This class is a combination between a singleton
        /// and a regular class to allow for the developers to have more customability.
        /// 
        /// One thing worth mentioning, the only thing that this system will not allow developers to
        /// customize is the use of consecutive integers for log levels starting at 0.  Developers are
        /// free to use however many levels they want, define them however they want, however... that is the
        /// one limitation I am comfortable placing on them.
        /// </summary>
        public class Logger
        {
            private static Logger logger;
            protected static string[] logLevelDesc = null;

            protected LoggerEventHandler[][] leh;
            protected uint max = 0;
            protected uint levels = 0;
            protected LoggerEventHandler defaultHandler = null;

            /// <summary>
            /// With this constructor the developer is responsible for defining what
            /// they want the logger to do (in the defaultHandler).
            /// </summary>
            /// <param name="levels"></param>
            /// <param name="defaultHandler"></param>
            public Logger(uint levels, LoggerEventHandler defaultHandler)
            {
                init(levels, defaultHandler);
            }

            /// <summary>
            /// Opens up a standard to file logger with the specified number of log levels.
            /// </summary>
            /// <param name="levels"></param>
            /// <param name="filename"></param>
            public Logger(uint levels, string filename)
            {
                init(levels, new BasicFileLogEventHandler(filename));
            }

            /// <summary>
            /// User specifies only the number of log levels they require.  They are still obligated
            /// (assuming they want the logger to do something) to add a handler.
            /// </summary>
            /// <param name="levels"></param>
            public Logger(uint levels)
            {
                init(levels, null);
            }

            /// <summary>
            /// Takes the current object and places it as the internal singleton reference
            /// </summary>
            /// <returns></returns>
            public Logger promoteToStatic()
            {
                logger = this;
                return logger;
            }

            /// <summary>
            /// The singleton can get set in two ways:
            /// 
            /// 1.) The developer promotes their class to be the Logger object inside the singleton
            /// 2.) We decide for them what their class is going to look like
            /// </summary>
            /// <returns></returns>
            public static Logger singleton()
            {
                if (logger == null)
                {
                    //use defaults
                    string logFile = DateTime.Now.ToShortDateString().Replace(@"/", @"-").Replace(@"\", @"-") + ".log";
                    logger = new Logger(6, logFile);
                    logLevelDesc = new string[6];
                    logLevelDesc[0] = "V_CRITICAL";
                    logLevelDesc[1] = "V_ERROR";
                    logLevelDesc[2] = "V_WARN";
                    logLevelDesc[3] = "V_INFO";
                    logLevelDesc[4] = "V_DEBUG";
                    logLevelDesc[5] = "V_ALL";
                }

                return logger;
            }

            /// <summary>
            /// Effectively the constructor (so no code would have to be repeated).  Consequently
            /// if one were to try to put this block inside the constructor matching the signature and
            /// then referenced it with :this(levels, defaultHandler) in the other constructors MS
            /// has a bit of a fit.  Dunno...this works, don't really care.
            /// </summary>
            /// <param name="levels"></param>
            /// <param name="defaultHandler"></param>
            private void init(uint levels, LoggerEventHandler defaultHandler)
            {
                this.levels = levels;
                this.defaultHandler = defaultHandler;
                this.max = levels - 1;

                leh = new LoggerEventHandler[levels][];

                LoggerEventHandler[] handler = new LoggerEventHandler[1];

                handler[0] = defaultHandler;

                for (int i = 0; i < levels; i++)
                {
                    leh[i] = handler;
                }
            }


            /// <summary>
            /// This is the maximum level which will trigger logging.
            /// </summary>
            /// <param name="min"></param>
            public void setMaximumLogLevel(uint max)
            {
                this.max = max;
            }

            /// <summary>
            /// Retrieve maximum logging level (note: this is not the total number of levels, but rather
            /// the upper bound where an action will or won't take placed based on the log level)
            /// </summary>
            /// <returns></returns>
            public uint getMaximumLogLevel()
            {
                return max;
            }

            /// <summary>
            /// Retreives the default handler if one is set
            /// </summary>
            /// <returns></returns>
            public LoggerEventHandler getDefaultLoggerEventHandler()
            {
                return this.defaultHandler;
            }

            /// <summary>
            /// Adds a customized log handler to each log level
            /// </summary>
            /// <param name="handler"></param>
            public void addSpecialLoggerToAllLevels(LoggerEventHandler handler)
            {
                if (handler == null) return;

                for (int level = 0; level < this.levels; level++)
                {
                    addSpecialLogger(level, handler);
                }
            }

            /// <summary>
            /// Adds a customized logger handler (e.g. log to file) to a specific level.
            /// Note: You can have (n log handlers assuming your system has the resources)
            /// </summary>
            /// <param name="level"></param>
            /// <param name="handler"></param>
            public void addSpecialLogger(int level, LoggerEventHandler handler)
            {
                if (level < levels)
                {
                    if (leh[level] != null)
                    {
                        int size = leh[level].Length + 1;
                        LoggerEventHandler[] temp = new LoggerEventHandler[size];

                        for (int i = 0; i < leh[level].Length; i++)
                        {
                            temp[i] = leh[level][i];
                        }

                        temp[size - 1] = handler;

                        leh[level] = temp;
                    }
                    else
                    {
                        leh[level] = new LoggerEventHandler[1];
                        leh[level][0] = handler;
                    }
                }
            }

            /// <summary>
            /// Sugared method to add a simple file handler method
            /// </summary>
            /// <param name="level"></param>
            /// <param name="filename"></param>
            public void addSpecialLogger(int level, string filename)
            {
                addSpecialLogger(level, new BasicFileLogEventHandler(filename));
            }

            /// <summary>
            /// Invokes all appropriate log event handlers with the message
            /// </summary>
            /// <param name="level"></param>
            /// <param name="tag"></param>
            /// <param name="message"></param>
            public void log(int level, string tag, string message)
            {
                if ((level <= max) && (level < levels) && (leh[level] != null))
                {
                    for (int i = 0; i < leh[level].Length; i++)
                    {
                        if (logLevelDesc == null)
                        {
                            if (leh[level][i] != null) leh[level][i].log(tag, level, "", message);
                        }
                        else
                        {
                            if (leh[level][i] != null) leh[level][i].log(tag, level, logLevelDesc[level], message);
                        }
                    }
                }
            }

            /// <summary>
            /// Invokes the shutdown method for all log handlers
            /// </summary>
            public void shutdown()
            {
                //This is not the most efficient, really I'd like to add a collection
                //in this class and loop through it (save some repeats), also allow for some other neet stuff.
                for (int level = 0; level < leh.Length; level++)
                {
                    for (int i = 0; i < leh[level].Length; i++)
                    {
                        if (leh[level][i] != null) leh[level][i].shutdown();
                    }
                }
            }
        }
    }
}
