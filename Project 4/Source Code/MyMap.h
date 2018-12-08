// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#ifndef MYMAP_H
#define MYMAP_H

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap() {
        m_root = NULL;
        m_size = 0;
    }
    
    ~MyMap()
    { clear();
    }
    
    void clear()
    {
        recursiveDelete(m_root);
    }
    
    int size() const
    {
        return m_size;
    }
    
    void associate(const KeyType& key, const ValueType& value)
    {
        ++m_size;
        if(m_root == NULL)
        {
            m_root = new Node;
            m_root->m_key = key;
            m_root->m_value = value;
            m_root->left = NULL;
            m_root->right = NULL;
            return;
        }
        
        Node* insert = m_root;
        
        for(;;)
        {
            if(key == insert->m_key)
            {
                insert->m_key = key;
                insert->m_value = value;
                return;
            }
            else if(key < insert->m_key)
            {
                if(insert->left != NULL)
                    insert = insert->left;
                else
                {
                    insert->left = new Node;
                    insert->left->m_key = key;
                    insert->left->m_value = value;
                    insert->left->left = NULL;
                    insert->left->right = NULL;
                    return;
                }
            }
            else if(key > insert->m_key)
            {
                if(insert->right != NULL)
                    insert = insert->right;
                else
                {
                    insert->right = new Node;
                    insert->right->m_key = key;
                    insert->right->m_value = value;
                    insert->right->left = NULL;
                    insert->right->right = NULL;
                    return;
                }
            }
        }
    }
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const
    {
        if(m_root == NULL)
        {
            return NULL;
            
        }
        
        Node* find = m_root;
        
        for(;;)
        {
            if(key == find->m_key)
                return &(find->m_value);
            if(key < find->m_key)
            {
                if(find->left != NULL)
                    find = find->left;
                else
                {
                    return NULL;
                }
            }
            else if(key > find->m_key)
            {
                if(find->right != NULL)
                    find = find->right;
                else
                {
                    return NULL;
                }
            }
        }
    }
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    struct Node
    {
        Node* left;
        Node* right;
        
        KeyType m_key;
        ValueType m_value;
    };
    
    int m_size;
    
    Node* m_root;
    
    // helper function(s)
    
    void recursiveDelete(Node* data)
    {
        if(data == NULL)
            return;
        recursiveDelete(data->left);
        recursiveDelete(data->right);
        
        delete data;
    }
};

#endif
