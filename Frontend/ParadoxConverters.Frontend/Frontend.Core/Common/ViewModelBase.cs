﻿using Caliburn.Micro;
using System;

namespace Frontend.Core.Common
{
    public abstract class ViewModelBase : PropertyChangedBase, IDisposable
    {
        private IEventAggregator eventAggregator;
        private bool isDisposed;

        protected ViewModelBase(IEventAggregator eventAggregator)
        {
            this.eventAggregator = eventAggregator;

            //HACK: This needs rethinking, but when vms gets resolved from a view (Say, a contentcontrol binding to a vm), 
            //the Load method obviously won't get called.
            //this.Load(null);
        }

        protected IEventAggregator EventAggregator
        {
            get
            {
                return this.eventAggregator;
            }
        }

        public void Load(object parameter)
        {
            this.OnLoading(parameter);
            this.OnLoad(parameter);
            this.OnLoaded(parameter);
        }

        public void Unload()
        {
            this.OnUnloading();
            this.OnUnload();
            this.OnUnloaded();
        }

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            if (isDisposing && !this.isDisposed)
            {
                this.Unload();
                this.isDisposed = true;
            }
        }

        protected virtual void OnLoading(object parameter)
        {
        }

        protected virtual void OnLoad(object parameter)
        {
        }

        protected virtual void OnLoaded(object parameter)
        {
        }

        protected virtual void OnUnloading()
        {
        }

        protected virtual void OnUnload()
        {
        }

        protected virtual void OnUnloaded()
        {
        }
    }
}
