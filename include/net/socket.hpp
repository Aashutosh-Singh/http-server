#pragma once
class Socket{
    private:
        int fd_;
    public:
        Socket()noexcept;

        explicit Socket(int fd);

        ~Socket();

        Socket(const Socket&)=delete;
        Socket& operator=(const Socket& other)=delete;

        Socket(Socket&& other)noexcept;
        Socket& operator=(Socket&& other)noexcept;
        [[nodiscard]]
        int fd()const noexcept; //const gaurantee that calling fd will not modify
                    //internal state
        [[nodiscard]]
        bool is_valid()const noexcept;

        void reset(int fd=-1)noexcept;

        [[nodiscard]]
        int release()noexcept;

        void swap(Socket& other)noexcept;
        
};