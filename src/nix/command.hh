#pragma once

#include "args.hh"

namespace nix {

/* A command is an argument parser that can be executed by calling its
   run() method. */
struct Command : virtual Args
{
    virtual std::string name() = 0;
    virtual void prepare() { };
    virtual void run() = 0;
};

class Store;

/* A command that require a Nix store. */
struct StoreCommand : virtual Command
{
    bool reserveSpace;
    StoreCommand(bool reserveSpace = true)
        : reserveSpace(reserveSpace) { };
    void run() override;
    virtual void run(ref<Store>) = 0;
};

typedef std::map<std::string, ref<Command>> Commands;

/* An argument parser that supports multiple subcommands,
   i.e. ‘<command> <subcommand>’. */
struct MultiCommand : virtual Args
{
    Commands commands;

    std::shared_ptr<Command> command;

    MultiCommand(const Commands & commands);

    void printHelp(const string & programName, std::ostream & out) override;

    bool processFlag(Strings::iterator & pos, Strings::iterator end) override;

    bool processArgs(const Strings & args, bool finish) override;
};

/* A helper class for registering commands globally. */
struct RegisterCommand
{
    static Commands * commands;

    RegisterCommand(ref<Command> command)
    {
        if (!commands) commands = new Commands;
        commands->emplace(command->name(), command);
    }
};

}
