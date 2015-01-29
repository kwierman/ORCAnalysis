#ifndef Protium_Daemon_hh_
#define Protium_Daemon_hh_

namespace Protium{
	namespace Threads{


		class Daemon{
		private:
			pid_t pid;
			pid_t sid;
		public:
			Daemon(){
				pid=0;
			}

			void start(){

				pid=fork();
			}

			bool isGood(){
				return (pid!=-1) && (pid!=0);
			}

			void killGrandParent(){
				exit(EXIT_SUCCESS);
			}

			void setFileMask(){
				umask(0);
			}
			void setSID(){
				sid=setsid();
				if(sid<0)
					exit(EXIT_FAILURE);
			}

			void setToRootFile(){
				if( chdir("/")<0){
					exit(EXIT_FAILURE);
				}
			}

			void closeFileDescriptors(){
		        close(STDIN_FILENO);
		        close(STDOUT_FILENO);
        		close(STDERR_FILENO);
			}

			void loop(){

				//schedule tasks and whatnot
			}
		};
	}
}


#endif //File Guardian