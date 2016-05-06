#include <unordered_set>
#include <functional>
#include <memory>
#include <iostream>
#include <string>

//**************************************************************************************************
// Here Starts the Generic Subject/Observer implementation
//**************************************************************************************************

template <typename... Args>
class Subject;

template <typename... Args>
class Observer {
   public:
    /**
     * \brief Constructor
     * \param  subject   Subject this observer is subscribed to.
     * \param  callback  Reference to callback function.
     */
    Observer(Subject<Args...>* subject, const std::function<void(Args...)> callback)
        : subject_(subject), callback_(callback)
    {
    }
    /**
     * \brief  Destructor
     */
    ~Observer()
    {
        if (subject_) {
            subject_->Detach(*this);
        }
    }
    /**
     * \brief Update the observer.
     * \param  args  Observer callback argument.
     */
    void Update(Args... args)
    {
        callback_(args...);
    };
    /**
     * \brief Subject detached this observer.
     */
    void Detached()
    {
        subject_ = nullptr;
    }
    /**
     * \brief Checks if the observer is attached.
     * \return  true if subject is attached.
     */
    bool IsAttached() const
    {
        return (subject_ != nullptr);
    }

   private:
    /**
     * Subject that this observer is subscribed to.
     */
    Subject<Args...>* subject_;
    /**
     * Callback function.
     */
    const std::function<void(Args...)> callback_;
};

template <typename... Args>
class Subject {
   public:
    /**
     * \brief  Destructor.
     *         Safely detaches all observers.
     */
    ~Subject()
    {
        for (auto& observer : observer_set_) {
            observer->Detached();
        }
    }
    /**
     * \brief Attach a callback to subject.
     * \param  callback  Reference to callback function.
     * \return  A new Observer subscribed to this subject.
     */
    std::unique_ptr<const Observer<Args...>> Attach(const std::function<void(Args...)>& callback)
    {
        auto* observer = new Observer<Args...>(this, callback);
        observer_set_.insert(observer);
        return std::unique_ptr<const Observer<Args...>>(observer);
    }
    /**
     * \brief Detach a observer from this subject.
     * \param  observer  Observers to be detached.
     */
    void Detach(Observer<Args...>& observer)
    {
        observer_set_.erase(&observer);
    }
    /**
     * \brief Notifies the observers.
     * \param  args  Values to be passed to the observers.
     */
    void Notify(Args... args) const
    {
        for (auto& observer : observer_set_) {
            observer->Update(args...);
        }
    }

   private:
    /**
     * Unordered set of subscribed observers.
     */
    std::unordered_set<Observer<Args...>*> observer_set_;
};

//**************************************************************************************************
// Here ends the Subject/Observer implementation
//**************************************************************************************************

//**************************************************************************************************
// Here Starts an example
//**************************************************************************************************

class Listener {
   public:
    Listener(Subject<int, int>& subject)
        // Using a lambda to crate a closure with this instance. This way we can pass a non static
        // member funciton as a callback.
        : name(),
          obs_(subject.Attach([this](int a, int b) { Callback(a, b); }))
    {
    }
    void Callback(int a, int b)
    {
        std::cout << name << a + b << std::endl;
    }
    static void StaticCallback(int a, int b)
    {
        std::cout << "Static Callback " << a + b << std::endl;
    }

    std::string name;

   private:
    std::unique_ptr<const Observer<int, int>> obs_;
};

int main(int argc, char* argv[])
{
    std::cout << "Listener Observer Test!" << std::endl;

    // Creates a Subject
    auto* subject = new Subject<int, int>();

    // Register a lambda
    auto obs
        = subject->Attach([](int a, int b) { std::cout << "Lambda (+)" << a + b << std::endl; });

    // Notifies the Observers...
    subject->Notify(1, 1);

    // We can delete the observer to stop listening to it
    obs.reset();
    subject->Notify(1, 2);

    // The subject notifies it's observers if itself is deleted

    // Register another lambda and a static callback
    obs = subject->Attach([](int a, int b) { std::cout << "Lambda (-) " << a - b << std::endl; });
    auto obs2 = subject->Attach(Listener::StaticCallback);
    // Notifies the Observers...
    subject->Notify(2, 1);

    // We can check if our observer is still attached to the subject
    std::cout << "Observer is attached = " << (obs->IsAttached() ? "true" : "false") << std::endl;

    delete subject;
    // Subject deleted, we can see our observer is no longer attached
    std::cout << "Observer is attached = " << (obs->IsAttached() ? "true" : "false") << std::endl;
    // And we can safely delete our observer
    obs.reset();
    obs2.reset();

    // Now let's recreate our subject
    subject = new Subject<int, int>();
    {
        // And create a class with Observer support
        auto listener = Listener(*subject);
        listener.name = "Listener ";
        subject->Notify(3, 1);

        // Listener uses a reference to the object so update the object will reflect the callback
        listener.name = "Updated listener ";
        subject->Notify(3, 2);
    }
    subject->Notify(3, 3);

    delete subject;
}
