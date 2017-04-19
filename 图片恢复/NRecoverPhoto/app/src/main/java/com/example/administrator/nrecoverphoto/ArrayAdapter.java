package com.example.administrator.nrecoverphoto;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.administrator.nrecoverphoto.bean.ImgBean;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;


/**
 * Created by Administrator on 2016/9/12/012.
 */
public abstract class ArrayAdapter<T> extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    private final int ITEM_TAG = 0x46257;
    private ArrayList<T> arrayObject;
    private Context mContext;
    private int mResource;
    private ArrayList<T> mOriginalValues;
    private final Object mLock = new Object();
    private LayoutInflater inflater;
    private boolean isHead;
    private MainAdapter.Listener listener;

    public ArrayAdapter(Context context) {
        init(context, 0, new ArrayList<T>());
    }

    public ArrayAdapter(Context context, int resource) {
        init(context, resource, new ArrayList<T>());
    }

    public ArrayAdapter(Context context, int resource, ArrayList<T> object) {
        init(context, resource, object);
    }

    private void init(Context context, int resource, ArrayList<T> arrayObject) {
        this.mContext = context;
        this.arrayObject = arrayObject;
        this.mResource = resource;
        inflater = LayoutInflater.from(context);
    }

    public void setOnClickListener(MainAdapter.Listener listener) {
        this.listener = listener;
    }

    public void isHead(boolean isHead) {
        this.isHead = isHead;
    }

    public Context getContext() {
        return mContext;
    }

    public void add(T object) {
        synchronized (mLock) {
            if (mOriginalValues != null) {
                mOriginalValues.add(object);
            } else {
                arrayObject.add(object);
            }
        }
        int index = arrayObject.size();
        if (mOriginalValues != null)
            index = mOriginalValues.size();
        notifyItemInserted(index);

    }

    public void addAll(Collection<? extends T> collection) {
        synchronized (mLock) {
            if (mOriginalValues != null) {
                mOriginalValues.addAll(collection);
            } else {
                arrayObject.addAll(collection);
            }
        }
        notifyDataSetChanged();

    }

    public void addAll(T... objects) {
        synchronized (mLock) {
            if (mOriginalValues != null) {
                Collections.addAll(mOriginalValues, objects);
            } else {
                Collections.addAll(arrayObject, objects);
            }
        }
        notifyDataSetChanged();
    }

    public void insert(T object, int index) {
        synchronized (mLock) {
            if (mOriginalValues != null) {
                mOriginalValues.add(index, object);
            } else {
                arrayObject.add(index, object);
            }
        }

        notifyDataSetChanged();
    }

    /**
     * Removes the specified object from the array.
     *
     * @param object The object to remove.
     */
    public void remove(T object) {
        synchronized (mLock) {
            if (mOriginalValues != null) {
                mOriginalValues.remove(object);
            } else {
                arrayObject.remove(object);
            }
        }
        notifyDataSetChanged();
    }

    public ArrayList<T> getData() {
        synchronized (mLock) {
            if (mOriginalValues != null)
                return mOriginalValues;
            else return arrayObject;
        }
    }

    public void clear() {
        synchronized (mLock) {
            if (mOriginalValues != null) {
                mOriginalValues.clear();
            } else {
                arrayObject.clear();
            }
        }
        notifyDataSetChanged();
    }


    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        RecyclerView.ViewHolder viewHolder = getViewHolder(parent, viewType);
        if (viewHolder != null) {
            return viewHolder;
        }

        if (mResource != 0) {
            View view = inflater.inflate(mResource, parent, false);
            return new RecyclerView.ViewHolder(view) {
            };
        }

        return null;
    }

    private void setListener(RecyclerView.ViewHolder holder) {
        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if (listener != null)
                {
                    listener.onClickListener((ImgBean) v.getTag());
                }
            }
        });

        holder.itemView.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                return false;
            }
        });
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        int p = position;
        if (isHead && p != 0)
            p = p - 1;
        T object = getItem(p);
        if (object != null) {
            holder.itemView.setTag(object);
            onBindView(holder, p, object);
            onBindView(holder, p);
            setListener(holder);
        }

    }


    @Override
    public int getItemCount() {

        if (arrayObject != null) {
            if (mOriginalValues != null)
                return mOriginalValues.size();
            return arrayObject.size();
        }
        return 0;
    }

    public T getItem(int position) {
        if (position != arrayObject.size())
            return arrayObject.get(position);
        else return null;
    }

    public void onBindView(RecyclerView.ViewHolder holder, int position) {

    }

    public abstract void onBindView(RecyclerView.ViewHolder holder, int position, T object);

    public abstract RecyclerView.ViewHolder getViewHolder(ViewGroup parent, int viewType);


}
