"use client";

import { useState } from "react";
import Link from "next/link";
import { Button } from "@/components/ui/button";
import { Sheet, SheetContent, SheetTrigger } from "@/components/ui/sheet";
import { Menu, LayoutDashboard, Cpu, Bell, Settings } from "lucide-react";
import { usePathname } from "next/navigation";

export default function Sidebar() {
  const [collapsed, setCollapsed] = useState(false);

  return (
    <>
      {/* Mobile top bar */}
      <div className="md:hidden sticky top-0 z-40 border-b bg-background p-2">
        <Sheet>
          <SheetTrigger asChild>
            <Button variant="outline" size="icon">
              <Menu className="h-5 w-5" />
            </Button>
          </SheetTrigger>
          <SheetContent side="left" className="p-0 w-72">
            <NavContent />
          </SheetContent>
        </Sheet>
      </div>

      {/* Desktop aside */}
      <aside
        className={`hidden md:flex h-screen sticky top-0 border-r bg-background transition-all duration-300 ${
          collapsed ? "w-20" : "w-64"
        }`}
      >
        <div className="flex flex-col w-full">
          {/* Top section */}
          <div className="flex items-center justify-between p-4">
            <span
              className={`font-bold text-lg transition-opacity ${
                collapsed ? "opacity-0" : "opacity-100"
              }`}
            >
              Welding
            </span>
            <Button
              variant="ghost"
              size="icon"
              onClick={() => setCollapsed((v) => !v)}
              className="ml-auto"
            >
              <Menu className="h-5 w-5" />
            </Button>
          </div>

          {/* Navigation */}
          <nav className="flex-1 space-y-1 px-2">
            <NavLinks collapsed={collapsed} />
          </nav>
        </div>
      </aside>
    </>
  );
}

function NavContent() {
  return (
    <div className="h-full bg-background">
      <div className="p-4 text-lg font-bold">Smart IIoT</div>
      <nav className="px-2 space-y-1">
        <NavLinks collapsed={false} />
      </nav>
    </div>
  );
}

function NavLinks({ collapsed }: { collapsed: boolean }) {
  const pathname = usePathname();
  const items = [
    { href: "/", label: "Overview", icon: LayoutDashboard },
    { href: "/devices", label: "Devices", icon: Cpu },
    { href: "/alerts", label: "Alerts", icon: Bell },
    { href: "/settings", label: "Settings", icon: Settings },
  ];

  return (
    <>
      {items.map(({ href, label, icon: Icon }) => {
        const active = pathname === href;
        return (
          <Link
            key={label}
            href={href}
            className={`flex items-center gap-3 rounded-md px-3 py-2 text-sm font-medium transition-colors 
              ${active ? "bg-primary text-black" : "hover:bg-muted"}
            `}
          >
            <Icon className="h-5 w-5 shrink-0" />
            {!collapsed && <span>{label}</span>}
          </Link>
        );
      })}
    </>
  );
}
