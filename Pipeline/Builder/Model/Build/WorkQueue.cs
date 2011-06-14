using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace Builder.Model
{
    public abstract class WorkQueue<T> : BasePropertyChanged where T : class
    {
        private Action m_workCompleted;
        private Action<int> m_progressChanged;

        private BackgroundWorker m_worker;
        private Queue<T> m_pending = new Queue<T>();
        private Queue<T> m_completed = new Queue<T>();

        private int m_totalPending = 0;
        private int m_totalCompleted = 0;

        public WorkQueue(Action<int> progressChanged, Action workCompleted)
        {
            m_progressChanged = progressChanged;
            m_workCompleted = workCompleted;

            m_worker = new BackgroundWorker();
            m_worker.DoWork += new DoWorkEventHandler(m_worker_DoWork);
            m_worker.WorkerReportsProgress = true;
            m_worker.ProgressChanged += new ProgressChangedEventHandler(m_worker_ProgressChanged);
            m_worker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(m_worker_RunWorkerCompleted);
        }

        void m_worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            DoCompletion();

            //The completion may have added new build items
            if (m_pending.Count > 0)
            {
                m_worker.RunWorkerAsync();
            }
            else
            {
                m_workCompleted();
            }
        }

        void m_worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            DoCompletion();
        }

        private void m_worker_DoWork(object sender, DoWorkEventArgs e)
        {
            T item;
            while (PopPending(out item))
            {
                Work(item);
                PushCompleted(item);
                m_worker.ReportProgress(0);
            }
        }

        protected abstract void Work(T item);
        protected abstract void Completed(T item);

        private void DoCompletion()
        {
            //Update total progress
            int totalComplete = m_totalCompleted;
            int totalPending = m_totalPending;

            if (totalPending != totalComplete)
                m_progressChanged((int)(((float)totalComplete / (float)totalPending) * 100.0f));
            else
                m_progressChanged(0);

            //Notify item completion
            T item;
            while (PopCompleted(out item))
            {
                Completed(item);
            }
        }

        protected void Enqueue(T item)
        {
            lock (m_pending)
            {
                m_pending.Enqueue(item);
            }

            ++m_totalPending;

            if (m_worker.IsBusy == false)
            {
                m_worker.RunWorkerAsync();
            }

            Changed("InProgress");
        }

        private void PushCompleted(T item)
        {
            lock (m_completed)
            {
                m_completed.Enqueue(item);
            }

            ++m_totalCompleted;
        }

        private bool PopCompleted(out T item)
        {
            item = null;
            lock (m_completed)
            {
                if (m_completed.Count > 0)
                    item = m_completed.Dequeue();
            }
            return item != null;
        }

        private bool PopPending(out T item)
        {
            item = null;
            lock (m_pending)
            {
                if (m_pending.Count > 0)
                    item = m_pending.Dequeue();
            }

            return item != null;
        }
    }
}
