// app/layout.tsx
import "./globals.css";
import Sidebar from "@/components/sidebar";

export default function RootLayout({ children }: { children: React.ReactNode }) {
  return (
    <html lang="en">
      <body className="flex min-h-screen bg-background">
        {/* Sidebar */}
        <Sidebar />

        {/* Main Content */}
        <main className="flex-1 p-6 overflow-y-auto">{children}</main>
      </body>
    </html>
  );
}
