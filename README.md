# City Manager – Phase 1

## Description
CLI application for managing city infrastructure reports.

## Roles
- inspector: can add reports, list, view, and filter
- manager: can remove reports and update threshold

## Commands
```bash
./city_manager --role inspector --user bob --add downtown
./city_manager --role inspector --user bob --add_report downtown
./city_manager --role inspector --user bob --list downtown
./city_manager --role inspector --user bob --view downtown 123
./city_manager --role manager --user alice --remove_report downtown 123
./city_manager --role manager --user alice --update_threshold downtown 3
./city_manager --role inspector --user bob --filter downtown severity>=2